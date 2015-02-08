#ifndef RUNNER_MEMORY_H
#define RUNNER_MEMORY_H

#include "debug.h"

#if _MSC_VER
    #if TDJX_DEBUG
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
    #else
        #include <stdlib.h>
    #endif
#else
    #include <stdlib.h>
#endif

#endif