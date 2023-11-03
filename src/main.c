#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"
#include "server.h"

#define UINT_DIGITS_CAP 16

int helloHandler(HTTPRequest *req, HTTPResponse *res) {
    static unsigned int view_count = 0;
    char buffer[UINT_DIGITS_CAP];
    HTTPResponse_write(res, "<h1>Hello world!</h1>");
    HTTPResponse_write(res, "<p>This page has been viewed ");
    int written = snprintf(buffer, UINT_DIGITS_CAP, "%d", view_count);
    buffer[written + 1] = '\0';
    HTTPResponse_write(res, buffer);
    HTTPResponse_write(res, " times</p>");
    view_count += 1;
    return 0;
}

int main(int argc, char **argv) {
    char ip[] = "127.0.0.1";
    unsigned short port = 8080;
    ChadtpServer *server = ChadtpServer_new(port, ip);
    if (server == NULL) {
        printf("EXITING...\n");
        return -1;
    }
    ChadtpServer_add_handler(server, helloHandler);
    printf("The server is now running on port %d\n", port);
    ChadtpServer_listen_and_serve(server);
    free(server);
    return 0;
}
