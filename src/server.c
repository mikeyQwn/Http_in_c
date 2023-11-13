#include <arpa/inet.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http/http.h"
#include "http/status_codes.h"
#include "parsing/parsing.h"
#include "path_matching.h"
#include "server.h"
#include "string_manipulation.h"

#define BUFSIZE 1024

// Returns NULL when the server creation has failed
// A constructor for the server
// You should always call it to initialize a server instance
ChadtpServer *ChadtpServer_new(unsigned short port, const char *ip) {
    ChadtpServer *server = (ChadtpServer *)malloc(sizeof(ChadtpServer));

    struct sockaddr_in servaddr;

    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sockfd == -1)
        return NULL;

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    int true = 1;
    setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(true));

    if ((bind(server->sockfd, (struct sockaddr *)&servaddr,
              sizeof(servaddr))) != 0)
        return NULL;

    server->handlers_length = 0;
    server->handlers_capacity = 5;
    server->handlers = (ChadtpHandler *)malloc(sizeof(ChadtpHandler) *
                                               server->handlers_capacity);

    return server;
}

// Returns NULL if it is not able to read the request or it has timed out
static char *read_request(int connfd, int *bytes_read) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(connfd, &read_fds);

    int res = select(connfd + 1, &read_fds, NULL, NULL, &timeout);
    if (res == -1) {
        return NULL;
    }
    size_t current_buff_size = sizeof(char) * 1;
    ssize_t actual_bytes_read;
    char *buff = malloc(current_buff_size);
    buff[0] = '\0';
    do {
        char temp[BUFSIZE];
        actual_bytes_read = read(connfd, temp, BUFSIZE);
        current_buff_size += actual_bytes_read;
        buff = realloc(buff, current_buff_size);
        strncat(buff, temp, actual_bytes_read);
        buff[current_buff_size] = '\0';
        res = select(connfd + 1, &read_fds, NULL, NULL, &timeout);
    } while (res > 0);
    *bytes_read = current_buff_size;

    return buff;
}

static int ChadtpServer_recieve_connection(ChadtpServer *self) {
    int connfd;
    struct sockaddr_in new_addr;
    socklen_t socklen = sizeof(new_addr);
    connfd = accept(self->sockfd, (struct sockaddr *)&new_addr, &socklen);
    return connfd;
}

static void ChadtpServer_log_request(ChadtpServer *self,
                                     HTTPRequest *parsed_request) {
    if (parsed_request == NULL) {
        printf("[server]Could not parse the request");
        return;
    }
    printf("METHOD: %s\nPATH: %s\nVERSION: %s\n",
           HTTPMethod_toString(parsed_request->method), parsed_request->path,
           HTTPVersion_toString(parsed_request->version));
    for (int i = 0; i < parsed_request->headers.length; ++i) {
        printf("KEY: %s, VALUE: %s\n", parsed_request->headers.headers[i].key,
               parsed_request->headers.headers[i].value);
    }
}

static void ChadtpServer_apply_handlers(ChadtpServer *self,
                                        HTTPRequest *parsed_request,
                                        HTTPResponse *http_response) {
    for (int i = 0; i < self->handlers_length; ++i) {
        ChadtpHandler handler = self->handlers[i];
        PathMatches matches = match_path(handler.path, parsed_request->path);
        if (matches.status == 0) {
            handler.f(parsed_request, http_response);
            break;
        }
        for (size_t i = 0; i < matches.wildcards_length; ++i)
            free(matches.wildcards[i]);

        free(matches.wildcards);
    }
}

static void ChadtpServer_write_status_line(ChadtpServer *self,
                                           HTTPResponse *http_response,
                                           StringBuffer *response_string) {
    StringBuffer_write(response_string, "HTTP/1.0 ");
    char *status_code_name = get_status_code_name(http_response->status_code);
    if (status_code_name == NULL) {
        StringBuffer_write_uint(response_string, 500);
        StringBuffer_append_char(response_string, ' ');
        StringBuffer_write(response_string, "Internal Server Error\n");
    } else {
        StringBuffer_write_uint(response_string, http_response->status_code);
        StringBuffer_append_char(response_string, ' ');
        StringBuffer_write(response_string, status_code_name);
        StringBuffer_append_char(response_string, '\n');
    }
}

static void ChadtpServer_write_headers(ChadtpServer *self,
                                       HTTPResponse *http_response,
                                       StringBuffer *response_string) {
    for (size_t i = 0; i < http_response->headers.length; ++i) {
        StringBuffer_write(response_string,
                           http_response->headers.headers[i].key);
        StringBuffer_write(response_string, ": ");
        StringBuffer_write(response_string,
                           http_response->headers.headers[i].value);
        StringBuffer_write(response_string, "\n");
    }
}

static void ChadtpServer_free_connection_leftovers(
    ChadtpServer *self, char *buff, HTTPResponse *http_response,
    StringBuffer *response_string, HTTPRequest *parsed_request) {
    free(buff);
    free(http_response->body.buffer);
    HTTPHeaders_free(&http_response->headers);

    free(response_string->buffer);
    if (parsed_request) {
        free(parsed_request->path);
        free(parsed_request->body);
        HTTPHeaders_free(&parsed_request->headers);
        free(parsed_request);
    }
}

// Accepts and handles a single connection
static int ChadtpServer_accept_connection(ChadtpServer *self) {
    int connfd = ChadtpServer_recieve_connection(self);
    if (connfd < 0) {
        close(connfd);
        return -1;
    }
    int bytes_read = 0;
    char *buff = read_request(connfd, &bytes_read);
    if (buff == NULL) {
        close(connfd);
        return -1;
    }
    printf("%s", buff);
    printf("\n---------\n");
    HTTPRequest *parsed_request = parse_request(buff);
    ChadtpServer_log_request(self, parsed_request);

    HTTPResponse http_response = HTTPResponse_new();
    ChadtpServer_apply_handlers(self, parsed_request, &http_response);

    const size_t RESPONSE_STRING_CAPACITY = 15;
    StringBuffer response_string =
        StringBuffer_with_capacity(RESPONSE_STRING_CAPACITY);

    ChadtpServer_write_status_line(self, &http_response, &response_string);
    ChadtpServer_write_headers(self, &http_response, &response_string);

    StringBuffer_append_char(&response_string, '\n');
    write(connfd, response_string.buffer, response_string.length);
    write(connfd, http_response.body.buffer,
          http_response.body.length * sizeof(char));
    printf("WRITTEN: %.*s\n", (int)http_response.body.length,
           http_response.body.buffer);

    close(connfd);
    ChadtpServer_free_connection_leftovers(self, buff, &http_response,
                                           &response_string, parsed_request);

    return 0;
}

// The run method for the HTTP server
int ChadtpServer_listen_and_serve(ChadtpServer *self) {
    if ((listen(self->sockfd, 5)) != 0)
        return -1;

    int server_is_running = 1;
    int error = 0;

    while (server_is_running) {
        int res = ChadtpServer_accept_connection(self);
        if (res == 1)
            break;
    }
    close(self->sockfd);
    return error;
}

static void sort_handlers(ChadtpServer *self) {
    int is_sorted = 1;
    do {
        ChadtpHandler top_handler = self->handlers[self->handlers_length - 1];
        is_sorted = 1;
        for (size_t i = 0; i < self->handlers_length - 1; ++i) {
            if (!is_more_specific(top_handler.path, self->handlers[i].path))
                continue;
            self->handlers[self->handlers_length - 1] = self->handlers[i];
            self->handlers[i] = top_handler;
            is_sorted = 0;
            break;
        }
    } while (!is_sorted);
}

void ChadtpServer_add_handler(ChadtpServer *self, char *template,
                              HandlerFunction f) {
    if (self->handlers_capacity == self->handlers_length) {
        self->handlers_capacity *= 2;
        self->handlers = realloc(self->handlers, self->handlers_capacity);
    }
    ChadtpHandler handler;
    handler.path = template;
    handler.f = f;
    self->handlers[self->handlers_length] = handler;
    self->handlers_length += 1;
    sort_handlers(self);
}
