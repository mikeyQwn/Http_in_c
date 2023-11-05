#ifndef CHADTP_PATH_MATCHING_H
#define CHADTP_PATH_MATCHING_H

#include <stddef.h>

typedef struct {
    int status;
    char **wildcards;
    size_t wildcards_length;
    size_t wildcards_capacity;
} PathMatches;

PathMatches match_path(char *, char *);
int is_more_specific(char *, char *);

#endif
