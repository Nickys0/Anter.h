// How to compile this file:
//    clang++ -std=c++20 -o tst tester.cpp -Ibil

#define BIL_DEF_CONFIG
#include "bil/bil.hpp"

using namespace bil;

#define TESTFILE_BIN "./.build/test"
#define TESTFILE_SRC "./tests/test.c"

log::basic_log tester_log("[" MK_HCYAN("info") "] ");

using strref = const std::string&; 
using ArgVec = const std::vector<CStr>&;

bool run_test(strref test_file, strref test_bin, types::Pecode expected, ArgVec args) {
    assert(fs::file_exist(test_file));

    tester_log << "Running test: [" << fs::base_name(test_file)<< "]\n";
    tester_log.log("");

    core::sh cmd;
    if(!fs::file_exist(test_bin) || fs::ft_newer(test_file, test_bin)){
        cmd << "{CC}" << "-Wall" << "-Wextra" << "-ggdb" 
            << "-o" << test_bin << test_file
            << "-I."
            << core::execeset;
    }
    cmd << test_bin << args;

    types::Pecode result;
    cmd.run(result, false);

    if(result != expected) std::cout << "Failed";
    else std::cout << "Succeed";

    tester_log << log::endl;
    
    return true;
}

int main(int argc, char** argv){
    log::info.disable( );
    log::warn.disable( );
    BIL_INIT(argc, argv);

    core::sh cmd;
    fs::mkdir_if_nexists("./.build/");

    run_test("./tests/basic_flags.c", "./.build/test_flag", 0, {"--bool", "--i32", "32", "-s", "Hello", "-d", "help"});
    run_test("./tests/basic_flags.c", "./.build/test_wrong_flag", 1, {"--wrong-bool"});
    run_test("./tests/commands.c", "./.build/test_flags", 0, {"new", "expected_value"});

    return 0;
}
