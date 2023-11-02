#ifndef CHADTP_PARSING_H
#define CHADTP_PARSING_H

#include <stddef.h>

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
    char **header_parts;
    size_t length;
    size_t capacity;
} HeaderPartVector;

HeaderPartVector *HeaderPartVector_new();
void HeaderPartVector_push(HeaderPartVector *, char *);

typedef struct {
    size_t length;
    HTTPHeader *headers;
} HTTPHeaders;

typedef struct {
    HTTPMethod method;
    char *path;
    HTTPVersion version;
    HTTPHeaders headers;
} HTTPRequest;

HTTPRequest *parse_request(char *);

#endif
