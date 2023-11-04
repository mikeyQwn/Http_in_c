#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header_parsing.h"
#include "parsing.h"

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

static void move_beyond_whitespaces(char **str) {
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

static char *parse_body(char **request) {
    char *start = *request;
    size_t len = 0;
    while (**request != '\0') {
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
    if (version == HTTP_VERSION_UNDEFINED) {
        free(path);
        return NULL;
    }
    while (*request == '\n' || *request == '\r') {
        ++request;
    }
    HTTPHeaders headers = parse_headers(&request);
    char *body = parse_body(&request);

    HTTPRequest *res = (HTTPRequest *)malloc(sizeof(HTTPRequest));
    res->method = method;
    res->path = path;
    res->version = version;
    res->headers = headers;
    res->body = body;
    return res;
}
