#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"

const char *HTTPMethod_toString(HTTPMethod self) {
    switch (self) {
    case HTTP_GET:
        return "GET";
    case HTTP_HEAD:
        return "HEAD";
    case HTTP_POST:
        return "POST";
    case HTTP_UNDEFINED:
        return "UNDEFINED";
    }
    exit(1);
}

const char *HTTPVersion_toString(HTTPVersion self) {
    switch (self) {
    case HTTP_VERSION_1_0:
        return "1.0";
    case HTTP_VERSION_1_1:
        return "1.1";
    case HTTP_VERSION_2_0:
        return "2.0";
    case HTTP_VERSION_3_0:
        return "3.0";
    case HTTP_VERSION_UNDEFINED:
        return "UNDEFINED";
    }
    exit(1);
}

static HTTPMethod parse_method(char **request) {
    if (!strncmp("GET", *request, 3)) {
        *request = *request + 3;
        return HTTP_GET;
    }
    if (!strncmp("HEAD", *request, 4)) {
        *request = *request + 4;
        return HTTP_HEAD;
    }
    if (!strncmp("POST", *request, 4)) {
        *request = *request + 4;
        return HTTP_POST;
    }
    return HTTP_UNDEFINED;
}

void move_beyond_whitespaces(char **str) {
    while (**str != '\0' && **str == ' ')
        ++*str;
}

static char *parse_path(char **request) {
    char *start = *request;
    size_t len = 0;
    while (**request != '\0' && **request != ' ') {
        ++*request;
        ++len;
    }
    if (len == 0)
        return NULL;
    char *res = malloc(sizeof(char) * (len + 1));
    res[len] = '\0';
    strncpy(res, start, len);
    return res;
}

static HTTPVersion parse_version(char **request) {
    if (strncmp("HTTP/", *request, 5))
        return HTTP_VERSION_UNDEFINED;
    *request = *request + 5;
    if (!strncmp("1.0", *request, 3)) {
        *request = *request + 3;
        return HTTP_VERSION_1_0;
    }
    if (!strncmp("1.1", *request, 3)) {
        *request = *request + 3;
        return HTTP_VERSION_1_1;
    }
    if (!strncmp("2.0", *request, 3)) {
        *request = *request + 3;
        return HTTP_VERSION_2_0;
    }
    if (!strncmp("3.0", *request, 3)) {
        *request = *request + 3;
        return HTTP_VERSION_3_0;
    }
    return HTTP_VERSION_UNDEFINED;
}

HTTPRequest *parse_request(char *request) {
    HTTPMethod method = parse_method(&request);
    if (method == HTTP_UNDEFINED)
        return NULL;
    move_beyond_whitespaces(&request);

    char *path = parse_path(&request);
    if (!path)
        return NULL;
    move_beyond_whitespaces(&request);

    HTTPVersion version = parse_version(&request);
    if (version == HTTP_VERSION_UNDEFINED)
        return NULL;

    HTTPRequest *res = (HTTPRequest *)malloc(sizeof(HTTPRequest));
    res->method = method;
    res->path = path;
    res->version = version;
    return res;
}
