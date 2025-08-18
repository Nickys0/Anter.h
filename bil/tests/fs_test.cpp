/// How to compile:
/// LINUX:
///     c++ -o -ggdb test fs_test.cpp -Wall
/// WINDOWS:
///     g++ -o -ggdb test fs_test.cpp -Wall

#include <iostream>
#define BIL_IMPLEMENTATION
#define __NAMESPACE_WEAK__
#include "bil.hpp"

int main(int argc, char** argv){

    std::string file_path_1 = "test1";
    std::string file_path_2 = "test2";
    std::string file_path_3 = "test2/test3";
    std::string file_path_4 = "test2/test3/file.txt";

    if(fs::mkdir_if_nexists(file_path_1)) return 1;
    if(fs::remove(file_path_1)) return 1;

    if(fs::mkdir_if_nexists_rec(file_path_3)) return 1;
    if(fs::create_file(file_path_4)) return 1;
    if(fs::remove_all(file_path_3)) return 1;
    if(fs::remove(file_path_2)) return 1;

    return 0;
}