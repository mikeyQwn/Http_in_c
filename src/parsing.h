#ifndef CHADTP_PARSING_H
#define CHADTP_PARSING_H

typedef enum { HTTP_GET, HTTP_HEAD, HTTP_POST, HTTP_UNDEFINED } HTTPMethod;
typedef enum {
    HTTP_VERSION_1_0,
    HTTP_VERSION_1_1,
    HTTP_VERSION_2_0,
    HTTP_VERSION_3_0,
    HTTP_VERSION_UNDEFINED
} HTTPVersion;

const char *HTTPMethod_toString(HTTPMethod);
const char *HTTPVersion_toString(HTTPVersion);

typedef struct {
    HTTPMethod method;
    char *path;
    HTTPVersion version;
} HTTPRequest;

HTTPRequest *parse_request(char *);

#endif
