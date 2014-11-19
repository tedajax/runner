#include "log.h"

void log_log(LogLevel level, const char* context, const char* msg) {
    _set_console_color_level(level);

    fprintf(stdout, "\n[%s] %s\n", context, msg);

    if (level == LOG_LEVEL_PANIC) {
        DEBUG_BREAKPOINT();
    }

    _reset_console_color();
}

void log_log_format(LogLevel level, const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(level, context, format, args);
    va_end(args);
}

void log_info(const char* context, const char* msg) {
    log_log(LOG_LEVEL_INFO, context, msg);
}

void log_info_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_INFO, context, format, args);
    va_end(args);
}

void log_warning(const char* context, const char* msg) {
    log_log(LOG_LEVEL_WARNING, context, msg);
}

void log_warning_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_WARNING, context, format, args);
    va_end(args);
}

void log_error(const char* context, const char* msg) {
    log_log(LOG_LEVEL_ERROR, context, msg);
}

void log_error_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_ERROR, context, format, args);
    va_end(args);
}

void log_panic(const char* context, const char* msg) {
    log_log(LOG_LEVEL_PANIC, context, msg);
}

void log_panic_format(const char* context, const char* format, ...) {
    va_list args;
    va_start(args, format);
    _log_formatv(LOG_LEVEL_PANIC, context, format, args);
    va_end(args);
}

// Private
void _log_formatv(LogLevel level, const char* context, const char* format, va_list args) {
    const size_t MAX_LENGTH = 256;
    char msg[256];

    vsnprintf(&msg[0], MAX_LENGTH, format, args);

    log_log(level, context, msg);
}

void _set_console_color_level(LogLevel level) {
    
}

void _reset_console_color() {

}