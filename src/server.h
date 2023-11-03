#ifndef CHADTP_SERVER_H
#define CHADTP_SERVER_H

#include <netinet/in.h>

#include "parsing/parsing.h"

typedef int (*HandlerFunction)(HTTPRequest *, HTTPResponse *);

typedef struct {
    int sockfd;
    HandlerFunction *handlers;
    size_t handlers_length;
    size_t handlers_capacity;
} ChadtpServer;

ChadtpServer *ChadtpServer_new(unsigned short, const char *);
void ChadtpServer_add_handler(ChadtpServer *, HandlerFunction);
int ChadtpServer_listen_and_serve(ChadtpServer *);

#endif
