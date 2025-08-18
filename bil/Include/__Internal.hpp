/**
 * 
 *      HEADER: Include/__Internal.hpp
 *      It defines some basic types usefull for
 *      the entire library
 *      
 *      NO FUNCTION DECLARED
 * **/

#ifndef __INTERNAL_HPP
#define __INTERNAL_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

#if defined(_WIN32) || defined(_WIN64)
#   define BIL_SYSTEM           "Windows"
#   define BIL_SYSTEM_WIN       1
#elif defined(__linux__)
#   define BIL_SYSTEM           "Linux"
#   define  BIL_SYSTEM_LINUX    1
#else
#   error "Platform not supported yet"
#endif

#if defined(__x86_64) || defined(__x86_64__)
#   define BIL_MACHINE          "x64"
#   define BIL_MACHINE_IS_x64   1
#else
#   define BIL_MACHINE          "x86"
#   define BIL_MACHINE_IS_x86   1
#endif

// Detecting GCC
#if defined(__GNUC__)
    #define BIL_COMPILER_GCC 1
#elif defined(__clang__)
    #define BIL_COMPILER_CLANG 1
#elif defined(_MSC_VER)
    #define BIL_COMPILER_MSVC 1
#elif defined(__INTEL_COMPILER)
    #define COMPILER_INTEL 1
#endif

#if defined(BIL_COMPILER_GCC)
#   if defined(BIL_SYSTEM_LINUX)
#       define BIL_C_COMPILER   "cc"
#       define BIL_CXX_COMPILER "c++"
#   elif defined(BIL_SYSTEM_WIN)
#       define BIL_C_COMPILER   "gcc"
#       define BIL_CXX_COMPILER "g++"
#   endif
#elif deifned(BIL_COMPILER_CLANG)
#   define BIL_C_COMPILER   "clang"
#   define BIL_CXX_COMPILER "clang++"
#endif


#ifdef BIL_SYSTEM_WIN
// #    define WIN32_LEAN_AND_MEAN
// #    define _WINUSER_
// #    define _WINGDI_
// #    define _IMM_
// #    define _WINCON_
// #    include <windows.h>
// #    include <direct.h>
// #    include <shellapi.h>
# error "Not available yet!"
#else
#    include <sys/types.h>
#    include <sys/wait.h>
#    include <sys/stat.h>
#    include <unistd.h>
#    include <fcntl.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <cassert>

#include "colors.h"


#ifdef WEAK_NAMESPACE
#   define __NAMESPACE_START inline namespace bil{
#else
#   define __NAMESPACE_START namespace bil{
#endif

#define __NAMESPACE_END      }    

#define _CONSTEXPR      _GLIBCXX20_CONSTEXPR
#define _NODISCARD      _GLIBCXX_NODISCARD
#define _NORETURN       _GLIBCXX_NORETURN
#define _MAYBE_UNUSED   [[maybe_unused]]

__NAMESPACE_START

inline namespace types{
	using CStr = 		const char*;
    using Error =       int;

    static constexpr inline auto _err = 1;
    static constexpr inline auto _ok =  0;

    using string_ref = const std::string&;

	using i8 = char;
	using i16 = short;
	using i32 = int;
	using i64 = long long;
	
	using u8 = 	unsigned char;
	using u16 = unsigned short;
	using u32 = unsigned int;
	using u64 = unsigned long long;
    
#ifdef _WIN32

    using Fd =              HANDLE; /* File Handle */
    using Proc =            HANDLE; /* Process Handle */
    using Pecode =          DWORD;  /* Process exit code */

    static constexpr inline auto INVALID_PROC = INVALID_HANDLE_VALUE;
    static constexpr inline auto INVALID_FD =   INVALID_HANDLE_VALUE;
    /* Is there an invalid Exit Code? */
    static constexpr inline auto INVALID_EXITCODE = (DWORD)INT32_MIN;
    /* Technically this is not correct but if a process returns 
        this is becouse something went wrong */

#else

    using Proc =            int;    /* Process descriptior */
    using Fd =              int;    /* File descriptior */
    using Pecode =          int;    /* Process exit code */
    
    static constexpr inline auto INVALID_PROC = -1;
    static constexpr inline auto INVALID_FD =   -1;
    /* Is there an invalid Exit Code? */
    static constexpr inline auto INVALID_EXITCODE = INT32_MIN;
    /* Technically this is not correct but if a process returns 
        this is becouse something went wrong */

#endif
    using Procs =           std::vector<Proc>;   /* Processes container */
    using Args =            std::vector<std::string>;   /* Arguments container */

    enum class Comparator: i32{
        UNKNOWN,    /* used for indicating an error */
        EQUAL,
        MORE,
        LESS
    };


    /* it describes the Process Status */
    enum class status{
        UNKNOWN = -1,
        FINISHED,
        RUNNING,
        STILL
    };
}

__NAMESPACE_END

#endif
