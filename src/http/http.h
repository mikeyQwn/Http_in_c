#ifndef CHADTP_HTTP
#define CHADTP_HTTP

#include <stddef.h>

#include "../string_manipulation.h"

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
    char *key;
    char *value;
} HTTPHeader;

typedef struct {
    size_t length;
    HTTPHeader *headers;
} HTTPHeaders;

typedef struct {
    HTTPMethod method;
    char *path;
    HTTPVersion version;
    HTTPHeaders headers;
    char *body;
} HTTPRequest;

typedef struct {
    unsigned int status_code;
    StringBuffer body;
    HTTPHeaders headers;
    size_t headers_capacity;
} HTTPResponse;

void HTTPHeaders_free(HTTPHeaders *);

HTTPResponse HTTPResponse_new();
void HTTPResponse_write(HTTPResponse *, const char *);
void HTTPResponse_write_status_code(HTTPResponse *, unsigned int);
void HTTPResponse_write_header(HTTPResponse *, const char *, const char *);

#endif
