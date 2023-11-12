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
    StringBuffer_write(&self->body, str);
}

void HTTPResponse_writen(HTTPResponse *self, const char *str, size_t len) {
    StringBuffer_writen(&self->body, str, len);
}

void HTTPResponse_write_status_code(HTTPResponse *self,
                                    unsigned int status_code) {
    self->status_code = status_code;
}

void HTTPResponse_write_header(HTTPResponse *self, const char *key,
                               const char *value) {
    if (self->headers_capacity <= self->headers.length) {
        self->headers_capacity *= 2;
        self->headers.headers =
            realloc(self->headers.headers,
                    sizeof(HTTPHeader *) * self->headers_capacity);
    }
    self->headers.headers[self->headers.length].key =
        copy_string(key, strlen(key));
    self->headers.headers[self->headers.length].value =
        copy_string(value, strlen(value));
    self->headers.length += 1;
}
