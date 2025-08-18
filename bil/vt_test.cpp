/// How to compile:
/// LINUX:
///     c++ -ggdb -std=c++20 -o test vt_test.cpp -Wall
/// WINDOWS:
///     g++ -ggdb -std=c++20 -o test vt_test.cpp -Wall

// #define __BIL_DBG_ST_TRACE__
// #define RESULT_FAST_FAILURE
#define WEAK_NAMESPACE
#define BIL_IMPLEMENTATION
#include "bil.hpp"

int main(int argc, char** argv){
    BIL_INIT(argc, argv);

    core::sh cmd;
 
    core::vtable::record("LIBS", "[pkg-config --libs sdl2]");
    core::vtable::record("OPT", "-ggdb");

    cmd << "cat" << "vt_test.cpp" << core::exec;

    return EXIT_SUCCESS;
}
