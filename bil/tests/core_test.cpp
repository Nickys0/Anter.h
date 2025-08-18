/// How to compile:
/// LINUX:
///     c++ -ggdb -std=c++20 -o test core_test.cpp -Wall
/// WINDOWS:
///     g++ -ggdb -std=c++20 -o test core_test.cpp -Wall

#include <iostream>
#define BIL_IMPLEMENTATION
#define __NAMESPACE_WEAK__
#include "../bil.hpp"

int main(void){
    core::Args cmd{"cat", "bil.hpp"};
    log::info.disable();

    fs::temp_path f{ };

    core::Fd_Rdr red{
        .fdout = core::open_file_for_write(f)
    };

    std::string contents{ };

    // log::info << "create_proc_async no redirecting" << log::endl;
    // core::create_proc_async(cmd);
    
    // log::info << "create_proc_sync no redirecting" << log::endl;
    // core::create_proc_sync(cmd);
    
    log::info << "create_proc_async redirecting" << log::endl;
    core::wait(core::create_proc_async(cmd, red));
    log::info << "OUT:\n\t" << f.read_entire_file( ) << log::endl;
    
    log::info << "create_proc_sync redirecting" << log::endl;
    contents = core::shout_stdout(cmd);
    log::info << "OUT:\n\t" << contents << log::endl;
    
    return EXIT_SUCCESS;
}

