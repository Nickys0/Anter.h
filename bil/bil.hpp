/**
 *      POSSIBILE DEFINES:
 *      - #define __BIL_DBG_ST_TRACE__:     [INTERNALLY USE!]
 *          This macro permit to print the stack trace if
 *          a function that return Result fails. to use this flag the program
 *          should statically link the -ldw library on linux
 *          !WARNING! this flag is not available on Windows!
 * 
 *      - #define RESULT_FAST_FAILURE:
 *          If this macro is enable it let a function that, return Result to 
 *          not return to the calle an rather exiting the program 
 * 
 *      - #define WEAK_NAMESPACE:
 *          It defines the bil namespace as inline so it can be avoided
 * 
 *      - #define BIL_IMPLEMENTATION:
 *          It should be defined at least one time to let the library have its definition
 * 
 *      - #define BIL_NO_DEFAULT_INDT:
 *          It disable the default functions logging indentation.
 * 
 *      EX:
 *      ```
 *      // Creating a Builder with the default config
 *      // se below for more details
 *      #define BIL_DEF_CONFIG
 *      #include "./bil.hpp"     
 * 
 *      int main(int argc, char** argv){
 *          BIL_INIT(argc, argv);
 *          core::sh cmd;
 *          // This should compile the test.c file
 *          cmd << "{CC}" << "-o" << "test" << "test.c" << "-Wall" << core::exec;
 *      }
 * 
 *      ```
 * 
 * 
 *            ----* VTABLE DEFAULT VARIABLES *----
 *      [CC]    => Common C Compiler, it is based on the platform and the compiler 
 *                  that was used to compile the builder
 *          WINDOWS =>  gcc | clang
 *          LINUX   =>  cc | clang
 * 
 *      [CXX]   => Like CC but for C++
 *          Windows =>  g++  | clang++
 *          Linux   =>  c++  | clang++
 *  
 * **/
#ifndef _BIL_HPP
#define _BIL_HPP

#if !defined(__cplusplus)
#   error "Error: This library is for C++ only!"
#endif

/* basic bil configuration */
#ifdef BIL_DEF_CONFIG
#   define BIL_IMPLEMENTATION
#   define WEAK_NAMESPACE
#   define BIL_NO_DEFAULT_INDT
#elif defined(BIL_DEBUG_CONFIG) 
#   define RESULT_FAST_FAILURE
#   define WEAK_NAMESPACE
#   define BIL_NO_DEFAULT_INDT
#   define BIL_IMPLEMENTATION
#   define __BIL_DBG_ST_TRACE__
#endif

#ifdef BIL_IMPLEMENTATION
#   define BIL__FILES_IMPL
#   define BIL__LOG_IMPL
#   define BIL__PROC_IMPL
#   define BIL__VTABLE_IMPL
#   define BIL__CMD_IMPL
#   define BIL__ALGOR_IMPL
#   define BIL__RESULT_IMPL
#   define BIL__HANDLER_IMPL
#endif

/* main headers */
#include "Include/__Internal.hpp"
#include "Include/log.hpp"
#include "Include/core/result.hpp"

/* usefull function for the later headers */
#include "Include/core/algor.hpp"

#include "Include/fs.hpp"           /* bil filesystem */
#include "Include/core.hpp"         /* core functions */
#include "Include/core/vtable.hpp"  /* variable table */
#include "Include/core/cmd.hpp"     /* cmd class */
#include "Include/handler.hpp"      /* basic args handler */

#define BIL_INIT(argc, argv, ...)\
        do{\
            bil::core::go_rebuild_yourself(argc, argv, __FILE__);\
            bil::core::vtable::init();\
            if(bil::__args(argc, argv))\
                std::exit(1);\
        }while(0)

#define BIL_INIT_NO_HANDLER(argc, argv, ...)\
        do{\
            bil::core::go_rebuild_yourself(argc, argv, __FILE__);\
            bil::core::vtable::init();\
        }while(0)

#endif


/**
 * 
 *                      ---------------------------
 *                          -*- BIL LIBRARY -*-
 *                      ---------------------------
 *      
 *      classes:
 *          - core::sh          DEFINED IN: core/cmd.hpp
 *          - core::vtable      DEFINED IN: core/vtable.hpp
 *          - Result            DEFINED IN: core/result.hpp
 *          - log::basic_log    DEFINED IN: log.hpp
 *  
 *      Version Conventions:
 *      - Modifying comments does not update the version.
 *      - PATCH is incremented in case of a bug fix or refactoring without touching the API.
 *      - MINOR is incremented when new functions and/or types are added in a way that does
 *        not break any existing user code. 
 *      - MAJOR update should be just a periodic cleanup of the deprecated functions and types
 *        without really modifying any existing functionality.
 * 
 *      
 *      VERSION HISTORY
 * 
 *      |    DATE    |  VERSION  |              DESCRIPTION
 *      | (15/08/25) |   0.4.0   |  ++ fs::base_name        [FUNC]
 *      |            |           |
 *      | (10/08/25) |   0.3.0   |  +- core::sh             Modified the run function
 *      |            |           |  ++ core::sh::run_async  [FUNC]
 *      |            |           |  ++ fs::curdir()         [FUNC]
 *      |            |           |  ++ fs::read_dir()       [FUNC]
 *      |            |           |
 *      | (19/05/25) |   0.2.1   |  Adding some macros for compiler detection and
 *      |            |           |  basic bil configuration
 *      |            |           |
 *      | (08/05/25) |   0.2.0   |  ++ Include/handler.hpp  [FILE]
 *      |            |           |  ++ vtable::init         [FUNC]
 *      |            |           |
 *      | (24/04/25) |   0.1.0   |  First public release of the library
 * 
 * 
 * **/
