#ifndef RUNNER_DEBUG_H
#define RUNNER_DEBUG_H

void _internal_print_assert(const char* expr,
    const char* file,
    int line,
    const char* msg);

#endif

#ifdef _WIN32
    #define DEBUG_BREAKPOINT() __debugbreak();
#else
    #define DEBUG_BREAKPOINT() __asm__("int $3");
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

#if defined(DEBUG) || defined(_DEBUG)
    #define ASSERT ASSERT_ALWAYS
#else
    #define ASSERT(expr, msg) ((void)0)
#endif

#if defined(DEBUG) || defined(_DEBUG)
    #define IF_DEBUG(x) x
#else
    #define IF_DEBUG(x) ((void)0)
#endif

#define CALLOC(count, type) (type*)calloc(count, sizeof(type))