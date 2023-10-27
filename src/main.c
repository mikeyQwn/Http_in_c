#include "server.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char ip[] = "127.0.0.1";
    unsigned short port = 8080;
    ChadtpServer *server = ChadtpServer_new(port, ip);
    if (server == NULL) {
        printf("EXITING...\n");
        return -1;
    }
    printf("The server is now running on port %d\n", port);
    ChadtpServer_listen_and_serve(server);
    free(server);
    return 0;
}
