#ifndef CHADTP_STRING_MANIPULATION
#define CHADTP_STRING_MANIPULATION

#include <stddef.h>

typedef struct {
    char *buffer;
    size_t length;
    size_t capacity;
} StringBuffer;

StringBuffer StringBuffer_with_capacity(size_t);
void StringBuffer_write(StringBuffer *, const char *);
void StringBuffer_writen(StringBuffer *, const char *, size_t);
void StringBuffer_append_char(StringBuffer *, char);
void StringBuffer_write_uint(StringBuffer *, unsigned int);

char *copy_string(const char *, size_t);

#endif
