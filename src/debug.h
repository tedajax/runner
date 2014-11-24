#ifndef RUNNER_DEBUG_H
#define RUNNER_DEBUG_H

#include <stdbool.h>

void _internal_print_assert(const char* expr,
    const char* file,
    int line,
    const char* msg);

#endif

#ifdef _WIN32
    #define DEBUG_BREAKPOINT() __debugbreak();
#else
    #include <signal.h>
    #define DEBUG_BREAKPOINT() raise(SIGTRAP);
#endif

#define MULTILINE_MACRO_BEGIN() do {
#define MULTILINE_MACRO_END() } while (0);

#define ASSERT_ALWAYS(expr, msg) MULTILINE_MACRO_BEGIN()            \
    static bool _skipAssert = false;                                \
    if (!_skipAssert && !(expr)) {                                  \
        _internal_print_assert(#expr, __FILE__, __LINE__, msg);     \
        DEBUG_BREAKPOINT(); \
    }                                                               \
    MULTILINE_MACRO_END();

#define ALWAYS_DEBUG_ON_KEY(key) MULTILINE_MACRO_BEGIN()            \
    if (input_key_down(key)) {                                      \
        DEBUG_BREAKPOINT()                                          \
    }                                                               \
    MULTILINE_MACRO_END()

#if defined(DEBUG) || defined(_DEBUG)
    #define IF_DEBUG(x) x
    #define ASSERT ASSERT_ALWAYS
    #define DEBUG_ON_KEY ALWAYS_DEBUG_ON_KEY
#else
    #define IF_DEBUG(x) ((void)0)
    #define ASSERT(expr, msg) ((void)0)
    #define DEBUG_ON_KEY ((void)0)
#endif

#define CALLOC(count, type) (type*)calloc(count, sizeof(type))