#include <stdlib.h>

#include "path_matching.h"

int is_more_specific(char *templ_a, char *templ_b) {
    while (*templ_a != '\0' && *templ_b != '\0') {
        if (*templ_a == *templ_b) {
            templ_a += 1;
            templ_b += 1;
            continue;
        }
        if (*templ_b == '*')
            return 1;
        break;
    }
    return 0;
}

static void push_wildcard(PathMatches *matches, char *wildcard) {
    if (matches->wildcards_length == matches->wildcards_capacity) {
        matches->wildcards_capacity *= 2;
        matches->wildcards = realloc(
            matches->wildcards, sizeof(char *) * matches->wildcards_capacity);
    }
    matches->wildcards[matches->wildcards_length] = wildcard;
    matches->wildcards_length += 1;
}

static int match_path_rec(char *template, char *path, PathMatches *matches) {
    while (*path != '\0' && *template != '\0') {
        if (*path != *template) {
            if (*template != '*')
                return -1;
            size_t len = 0;
            while (*(path + len) != '\0') {
                int res = match_path_rec(template + 1, path + len, matches);
                if (res != 0) {
                    len += 1;
                    continue;
                }
                char *wildcard = (char *)malloc(sizeof(char) * (len + 1 + 1));
                wildcard[len + 1] = '\0';
                push_wildcard(matches, wildcard);
                return 0;
            }
            if (*(template + 1) == '\0') {
                char *wildcard = (char *)malloc(sizeof(char) * (len + 1 + 1));
                wildcard[len + 1] = '\0';
                push_wildcard(matches, wildcard);
                return 0;
            }
            return -1;
        }
        ++path;
        ++template;
    }
    if (*path != *template) {
        if (*(template + 1) == '\0') {
            char *wildcard = (char *)malloc(sizeof(char) * (1));
            wildcard[0] = '\0';
            push_wildcard(matches, wildcard);
            return 0;
        }
        return -1;
    }
    return 0;
}

PathMatches match_path(char *template, char *path) {
    PathMatches res;
    res.status = -1;
    res.wildcards_capacity = 2;
    res.wildcards_length = 0;
    res.wildcards = malloc(sizeof(char *) * res.wildcards_capacity);

    res.status = match_path_rec(template, path, &res);
    return res;
}
