#include <stdlib.h>
#include <string.h>

#include "http.h"

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

void HTTPResponse_write(HTTPResponse *self, const char *str) {
    size_t len = strlen(str);
    while (self->length + len > self->capacity) {
        self->capacity *= 2;
        self->buffer = realloc(self->buffer, sizeof(char) * self->capacity);
    }
    for (size_t i = 0; i < len; ++i) {
        self->buffer[self->length + i] = str[i];
    }
    self->length += len;
}