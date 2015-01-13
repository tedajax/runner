#if !(__cplusplus)
    #ifdef _WIN32
        #ifdef _MSC_VER
            #define inline __inline
        #else
            #define inline inline
        #endif
    #else
        #define inline __inline__
    #endif
#endif
