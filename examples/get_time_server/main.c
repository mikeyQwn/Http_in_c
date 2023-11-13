#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../src/http/status_codes.h"
#include "../../src/server.h"

char *get_time() {
    time_t now;
    struct tm *timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    return asctime(timeinfo);
}

int not_found_handler(HTTPRequest *req, HTTPResponse *res) {
    HTTPResponse_write_status_code(res, CHADTP_NOT_FOUND);
    HTTPResponse_write(res, "<h1>This page is not found</h1><p>Please, use "
                            "/time for the current time</p>");
    return 0;
}

int time_handler(HTTPRequest *req, HTTPResponse *res) {
    HTTPResponse_write_status_code(res, CHADTP_OK);
    HTTPResponse_write(res, "<h1>Current time is</h1></div>");
    char *current_time = get_time();
    HTTPResponse_write(res, current_time);
    HTTPResponse_write(res, "</div>");
    return 0;
}

int main() {
    const char *IP = "127.0.0.1";
    const unsigned short PORT = 8080;
    ChadtpServer *server = ChadtpServer_new(PORT, IP);
    ChadtpServer_add_handler(server, "/time", time_handler);
    ChadtpServer_add_handler(server, "*", not_found_handler);
    ChadtpServer_listen_and_serve(server);

    free(server);
}
