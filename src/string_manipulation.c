#include "string_manipulation.h"

#include <stdlib.h>
#include <string.h>

void StringBuffer_writen(StringBuffer *self, const char *str, size_t len) {
    while (self->length + len > self->capacity) {
        self->capacity *= 2;
        self->buffer = realloc(self->buffer, sizeof(char) * self->capacity);
    }
    for (size_t i = 0; i < len; i++)
        self->buffer[self->length + i] = str[i];
    self->length += len;
}
void StringBuffer_write(StringBuffer *self, const char *str) {
    size_t len = strlen(str);
    StringBuffer_writen(self, str, len);
}
void StringBuffer_append_char(StringBuffer *self, char c) {
    if (self->length == self->capacity) {
        self->capacity *= 2;
        self->buffer = realloc(self->buffer, sizeof(char) * self->capacity);
    }
    self->buffer[self->length] = c;
    self->length += 1;
}

void StringBuffer_write_uint(StringBuffer *self, unsigned int value) {
    size_t value_len = 0;
    for (unsigned int temp = value; temp > 0; temp /= 10)
        ++value_len;

    while (self->length + value_len > self->capacity) {
        self->capacity *= 2;
        self->buffer = realloc(self->buffer, sizeof(char) * self->capacity);
    }

    for (size_t i = 0; i < value_len; i++) {
        self->buffer[self->length + value_len - i - 1] = '0' + (value % 10);
        value /= 10;
    }
    self->length += value_len;
}
