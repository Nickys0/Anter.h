/**
 *  Testing if both Files and Directories are threaten the same 
 * **/

#include <filesystem>
#include <iostream>

using File_Path = std::filesystem::path;

inline bool file_exist(const File_Path& path){
    return std::filesystem::exists(path);
}

int main(int argc, char** argv){

    std::string file_path;
    if(argc > 1){
        file_path = argv[1];
    }else{
        std::cerr << "Please provide a file!" << std::endl;
        exit(1);
    }

    std::cout << "Checking the file: " << file_path << std::endl;

    if(file_exist(file_path)){
        std::cout << "The file exist!" << std::endl;
    }else{
        std::cout << "The file do not exist!" << std::endl;
    }

    return 1;
}