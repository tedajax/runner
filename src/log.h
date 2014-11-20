#ifndef RUNNER_LOG_H
#define RUNNER_LOG_H

#include <stdarg.h>
#include <stdio.h>

#include "types.h"
#include "debug.h"

typedef enum log_level_e {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_PANIC
} LogLevel;

// Public
void log_log(LogLevel level, const char* context, const char* msg);
void log_log_format(LogLevel level, const char* context, const char* format, ...);

void log_info(const char* context, const char* msg);
void log_info_format(const char* context, const char* format, ...);

void log_warning(const char* context, const char* msg);
void log_warning_format(const char* context, const char* format, ...);

void log_error(const char* context, const char* msg);
void log_error_format(const char* context, const char* format, ...);

void log_panic(const char* context, const char* msg);
void log_panic_format(const char* context, const char* format, ...);

// Private
void _log_formatv(LogLevel level, const char* context, const char* format, va_list args);
void _set_console_color_level(LogLevel level);
void _reset_console_color();

#endif