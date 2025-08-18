#include "Include/core/result.hpp"
#include <iostream>

bil::Result only_func(void){
    return bil::make_result();
}

bil::Result int_func(int val){
    return bil::make_result(val);
}

int main(void){
    auto res = only_func();
    
    /* Not using it */
    int_func(EIDRM); /* discarding the Result results in a abort termination */

    std::cout << "This should be Unreachable" << std::endl;

    // if(_res){
    //     std::cerr << "Err" << std::endl;
    //     return 0;
    // }else{
    //     std::cout << "Success" << std::endl;
    // }

    return 0;
}