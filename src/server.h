#ifndef CHADTP_SERVER_H
#define CHADTP_SERVER_H

#include <netinet/in.h>

typedef struct {
    int sockfd;
} ChadtpServer;

ChadtpServer *ChadtpServer_new(unsigned short, const char *);
int ChadtpServer_listen_and_serve(ChadtpServer *);

#endif
