#ifndef LAB3_EXPORT_H
#define LAB3_EXPORT_H

#ifdef _WIN32
    #ifdef LAB3_STATIC
        #define LAB3_API
    #elif defined(LAB3_BUILD)
        #define LAB3_API __declspec(dllexport)
    #else
        #define LAB3_API __declspec(dllimport)
    #endif
#else
    #define LAB3_API
#endif

#endif
