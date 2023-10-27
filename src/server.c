#include <arpa/inet.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.h"

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
};

// Accepts and handles a single connection
static int ChadtpServer_accept_connection(ChadtpServer *self) {
    int connfd;
    struct sockaddr_in new_addr;
    socklen_t socklen = sizeof(new_addr);
    connfd = accept(self->sockfd, (struct sockaddr *)&new_addr, &socklen);

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
    char ok_res[] = "HTTP/1.0 200 OK\n";
    write(connfd, ok_res, sizeof(ok_res));

    close(connfd);
    free(buff);

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
