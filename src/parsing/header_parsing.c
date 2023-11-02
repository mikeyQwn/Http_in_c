#include "parsing.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **header_parts;
    size_t length;
    size_t capacity;
} HeaderPartVector;

HeaderPartVector *HeaderPartVector_new() {
    HeaderPartVector *res =
        (HeaderPartVector *)malloc(sizeof(HeaderPartVector));
    res->length = 0;
    res->capacity = 5;
    res->header_parts = (char **)malloc(sizeof(char *) * res->capacity);
    return res;
}

void HeaderPartVector_resize(HeaderPartVector *self) {
    self->capacity *= 2;
    self->header_parts =
        realloc(self->header_parts, sizeof(char *) * self->capacity);
}

void HeaderPartVector_push(HeaderPartVector *self, char *value) {
    if (self->length == self->capacity)
        HeaderPartVector_resize(self);
    self->header_parts[self->length] = value;
    self->length += 1;
}

void HeaderPartVector_free(HeaderPartVector *self) {
    for (int i = 0; i < self->length; ++i) {
        free(self->header_parts[i]);
    };
    free(self->header_parts);
    free(self);
}

static int strip_string(char *dst, const char *src) {
    size_t len = 0;
    int met_space = 0;
    while (*src != '\n') {
        if (*src == ' ') {
            ++src;
            if (met_space)
                continue;
            *dst = ' ';
            dst++;
            len += 1;
            met_space = 1;
            continue;
        }
        met_space = 0;
        len += 1;
        *dst = *src;
        ++dst;
        ++src;
    }
    return len;
}

static char *copy_string_stripped(char *src, size_t len) {
    char *res = malloc(sizeof(char) * (len + 1));
    size_t actual_len = strip_string(res, src);
    res[actual_len] = '\0';
    return res;
}

static int should_return(char **head) {
    if (**head == '\r') {
        if (*(*head + 1) == '\n') {
            *head += 2;
            return 1;
        }
    }
    if (**head == '\n') {
        *head += 1;
        return 1;
    }
    return 0;
}

static char *copy_string(char *src, size_t len) {
    char *res = malloc(sizeof(char) * (len + 1));
    res[len] = '\0';
    strncpy(res, src, len);
    return res;
}

static size_t move_to_next_non_whitespace(char **str) {
    size_t newlines = 0;
    while (**str != '\0' && (**str == ' ' || **str == '\n' || **str == '\t')) {
        if (**str == '\n') {
            newlines += 1;
        }
        ++*str;
    }
    return newlines;
}

static int parse_header_line(char **head, HeaderPartVector *keys,
                             HeaderPartVector *values) {
    if (should_return(head))
        return 1;

    int should_continue = 1;
    int met_colon = 0;
    size_t from_delimeter = 0;
    char *start = *head;

    while (should_continue) {
        switch (**head) {
        case '\0':
            return -1;
        case '\r':
            *head += 1;
            if (**head == '\n') {
                *head += 1;
                if (met_colon) {
                    char *res = copy_string_stripped(start, from_delimeter);
                    HeaderPartVector_push(values, res);
                }
                return 0;
            }
        case '\n':
            *head += 1;
            return 0;
        case ':': {
            if (met_colon) {
                *head += 1;
                break;
            }
            char *res = copy_string(start, from_delimeter);
            HeaderPartVector_push(keys, res);
            *head += 1;
            met_colon = 1;
            from_delimeter = 0;
            // FIXME: potential newline
            move_to_next_non_whitespace(head);
            start = *head;
            break;
        }
        default:
            *head += 1;
        }
        from_delimeter += 1;
    }
    return 0;
}

HTTPHeaders parse_headers(char *request) {
    HeaderPartVector *keys = HeaderPartVector_new();
    HeaderPartVector *values = HeaderPartVector_new();
    while (parse_header_line(&request, keys, values) == 0) {
    }
    HTTPHeaders headers;
    if (keys->length != values->length) {
        HeaderPartVector_free(keys);
        HeaderPartVector_free(values);
        headers.length = 0;
        return headers;
    }

    headers.length = keys->length;
    headers.headers = (HTTPHeader *)malloc(sizeof(HTTPHeader) * keys->length);
    for (int i = 0; i < keys->length; ++i) {
        HTTPHeader header;
        header.key = keys->header_parts[i];
        header.value = values->header_parts[i];
        headers.headers[i] = header;
    }

    free(keys->header_parts);
    free(keys);
    free(values->header_parts);
    free(values);
    return headers;
}
