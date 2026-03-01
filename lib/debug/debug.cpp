#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include <time.h>



static const char *file_basename(const char *path) {
    if (!path) {
        return "";
    }

    const char *p1  = strrchr(path, '/');
    const char *p2  = strrchr(path, '\\');
    const char *p   = p1 > p2 ? p1 : p2;

    return p ? (p + 1) : path;
}


void debug_init(uint32_t baud_rate) {
    Serial.begin(baud_rate);
    delay(10);
}


void debug_message(int level, const char *file, const char *func, int line, const char *fmt, ...) {
    if (level > SYSTEM_DEBUG_LEVEL) {
        return;
    }

    const char *color   = MESSAGE_COLOR_RESET;
    const char *prefix  = "UNKN";
    switch (level) {
        case DEBUG_LEVEL_CRITICAL:
            color = MESSAGE_COLOR_MAGENTA;  prefix  = "CRIT";   break;

        case DEBUG_LEVEL_ERROR:
            color = MESSAGE_COLOR_RED;      prefix  = "ERR ";   break;

        case DEBUG_LEVEL_WARNING:
            color = MESSAGE_COLOR_YELLOW;   prefix  = "WARN";   break;

        case DEBUG_LEVEL_INFO:
            color = MESSAGE_COLOR_GREEN;    prefix  = "INFO";   break;

        case DEBUG_LEVEL_TRACE:
            color = MESSAGE_COLOR_CYAN;     prefix  = "TRCE";   break;

        default:
            break;
    }


    // Datetime Formatting
    char datetime[BUFFER_SIZE_DATETIME];
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(datetime, sizeof(datetime), "%Y-%m-%d-%H-%M-%S", &timeinfo);


    // Message Allocation
    char buffer_message[BUFFER_SIZE_MESSAGE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer_message, sizeof(buffer_message), fmt, ap);
    va_end(ap);


    // File Name
    const char *filebase = file_basename(file);
    char header[BUFFER_SIZE_HEADER];


    // [datetime][PREFIX][file:line func] message
    snprintf(
        header, sizeof(header),
        "[%s][%s][%s:%d %s] ", datetime, prefix, filebase, line, func
    );

    char buffer_print[BUFFER_SIZE_PRINT];
    snprintf(
        buffer_print, sizeof(buffer_print),
        "%s%s%s%s\n", color, header, buffer_message, MESSAGE_COLOR_RESET
    );
    Serial.print(buffer_print);
}
