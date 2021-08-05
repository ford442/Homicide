#ifndef  __MAIN__HPP__
    #define __MAIN__HPP__

    #include <iostream>
    #include <cstring>

    #define DEBUG_LEVEL 6
    #define __func__ __FUNCTION__
    #define is_equal(str1, str2) !strcmp(str1, str2)

    #ifdef DEBUG_LEVEL
        #if (DEBUG_LEVEL >= 1)
            #define __CRT_ERR
        #endif

        #if (DEBUG_LEVEL >= 2)
            #define __ERR
        #endif

        #if (DEBUG_LEVEL >= 3)
            #define __WARN
        #endif

        #if (DEBUG_LEVEL >= 4)
            #define __ALLOC_LOG
        #endif

        #if (DEBUG_LEVEL >= 5)
            #define __LOAD_LOG
        #endif

        #if (DEBUG_LEVEL >= 6)
            #define __CONSTRUCT_LOG
            #define __DESTRUCT_LOG
        #endif
    #else
        #warning debug level not initalized, set as 0
    #endif

    #ifdef __CRT_ERR
        #define CRITICAL_ERR(msg) std::cerr << "CRITICAL ERROR :: in " << __func__ << " : reson : " << msg << std::endl;
        #undef __CRT_ERR
    #else
        #define CRITICAL_ERR(msg) {}
    #endif

    #ifdef __ERR
        #define ERR(msg) std::cerr << "ERROR :: in " << __func__ << " : reson : " << msg << std::endl;
        #undef __ERR
    #else
        #define ERR(msg) {}
    #endif
    
    #ifdef __WARN
        #define WARN(msg) std::cerr << "WARNING :: in " << __func__ << " : reson : " << msg << std::endl;
        #undef __WARN
    #else
        #define WARN(msg) {}
    #endif
    
    #ifdef __ALLOC_LOG
        #define ALLOC_LOG(msg) std::cout << "ALLOC INFO :: in " << __func__ << " : reson : " << msg << std::endl;
        #undef __ALLOC_LOG
    #else
        #define ALLOC_LOG(msg) {}
    #endif
    
    #ifdef __LOAD_LOG
        #define LOAD_LOG(msg) std::cout << "LOAD INFO :: in " << __func__ << " : load \"" << msg << "\" file" << std::endl;
        #undef __LOAD_LOG
    #else
        #define LOAD_LOG(msg) {}
    #endif

    #ifdef __CONSTRUCT_LOG
        #define CONSTRUCT() std::cout << "CONSTRUCT INFO :: construct a " << __func__ << " instance " << std::endl;
        #undef __CONSTRUCT_LOG
    #else
        #define LOAD_LOG() {}
    #endif

    #ifdef __DESTRUCT_LOG
        #define DESTRUCT() std::cout << "DESTRUCT INFO :: deconstruct a " << __func__ << " instance " << std::endl;
        #undef __DESTRUCT_LOG
    #else
        #define LOAD_LOG() {}
    #endif
    
#endif