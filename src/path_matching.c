#include "path_matching.h"

int match_path(char *template, char *path) {
    while (*path != '\0' && *template != '\0') {
        if (*path != *template)
            return -1;
        ++path;
        ++template;
    }
    if (*path != *template)
        return -1;
    return 0;
}
