#include <stdio.h>
#include <stdlib.h>

#include "../../src/server.h"

int echo_handler(HTTPRequest *req, HTTPResponse *res) {
    HTTPResponse_write(res, "<h1>");
    HTTPResponse_write(res, req->body);
    HTTPResponse_write(res, "</h1>");
    return 0;
}

int main(int argc, char **argv) {
    const unsigned short PORT = 8080;
    const char IP[] = "127.0.0.1";
    ChadtpServer *server = ChadtpServer_new(PORT, IP);
    if (server == NULL) {
        printf("Could not create a server, exiting\n");
        return -1;
    }
    ChadtpServer_add_handler(server, echo_handler);
    printf("The server is now running on port %d\n", PORT);
    ChadtpServer_listen_and_serve(server);
    free(server);
    return 0;
}
