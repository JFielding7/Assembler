#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_LINE_MESSAGE "ERROR: line %lu: "

void raise_compiler_error(char *message, ...) {
    va_list args;
    va_start(args, message);

    size_t base_len = strlen(ERROR_LINE_MESSAGE);
    size_t message_len = strlen(message);

    char error_message[base_len + message_len + 2];
    strncpy(error_message, ERROR_LINE_MESSAGE, base_len);
    strncpy(error_message + base_len, message, message_len);
    strcpy(error_message + base_len + message_len, "\n");

    vfprintf(stderr, error_message, args);
    va_end(args);

    exit(1);
}
