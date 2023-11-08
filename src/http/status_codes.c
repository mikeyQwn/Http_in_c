#include "status_codes.h"
#include "stddef.h"

char *get_status_code_name(unsigned int code) {
    switch (code) {
    case CHADTP_OK:
        return "OK";
    case CHADTP_CREATED:
        return "Created";
    case CHADTP_ACCEPTED:
        return "Accepted";
    case CHADTP_NO_CONTENT:
        return "No Content";
    case CHADTP_MOVED_PERMANENTLY:
        return "Moved Permanently";
    case CHADTP_MOVED_TEMPORARILY:
        return "Moved Temporarily";
    case CHADTP_NOT_MODIFIED:
        return "Not Modified";
    case CHADTP_BAD_REQUEST:
        return "Bad Request";
    case CHADTP_UNAUTHORIZED:
        return "Unauthorized";
    case CHADTP_FORBIDDEN:
        return "Forbidden";
    case CHADTP_NOT_FOUND:
        return "Not Found";
    case CHADTP_INTERNAL_SERVER_ERROR:
        return "Internal Server Error";
    case CHADTP_NOT_IMPLEMENTED:
        return "Not Implemented";
    case CHADTP_BAD_GATEWAY:
        return "Bad Gateway";
    case CHADTP_SERVICE_UNAVAILABLE:
        return "Service Unavailable";
    default:
        return NULL;
    }
}
