///     -*- HOW TO COMPILE -*-
/// c++ -std=c++20 -ggdb -o rd read_dir.cpp -ldw

// #define __BIL_DBG_ST_TRACE__
#define BIL_DEF_CONFIG
#include "./bil.hpp"

#define FILE_TO_COMPILE     "src/main.cpp"
#define OUTPUT_FILE_NAME    "ferry"

// This builder is available only on linux!
#if defined(BIL_SYSTEM_LINUX)
#define BUILD_FOLDER        ".build/"

#define TARGET_FOLDER       BUILD_FOLDER    "target"
#define OUT_FOLDER          TARGET_FOLDER   "/" BIL_MACHINE
#define OBJ_FOLDER          BUILD_FOLDER    "caches"

#define OUTPUT_FILE         OUT_FOLDER "/" OUTPUT_FILE_NAME


/////////////////////////////////////////////////////////////
//                          MAIN                          //
///////////////////////////////////////////////////////////
int main(int argc, char** argv){
    // log::info.disable( );
    auto info = log::New(nullptr);

    // Setting up handlers
    BIL_INIT(argc, argv);

    if(argc <= 1){
        log::err << "You should provide a dir to read!" << log::endl;
        std::exit(1);
    }

    fs::paths ph = fs::read_dir(argv[1]);
    assert(ph.size() != 0);

    log::info << "Total files: " << ph.size( ) << log::endl;

    for(const auto& i: ph)
        log::info << "File: " << i.c_str() << log::endl;
    

    return 0;
}


#endif