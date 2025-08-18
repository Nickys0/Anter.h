/**
 *      THIS FILE IMPLEMENTS SOME BASIC FUNCTIONS FOR
 *      HANDLING BASIC COMMANDS FO RTHE BIL LIBRARY
 * 
 *      FILE: Include/handler.hpp
 *      
 *      FUNCTIONS:
 *      - Result handle_args(int argc, char** argv):
 *          This function handles basic commands for 
 *          your builder.
 * 
 * **/
#ifndef _HANDLER_HPP
#define _HANDLER_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

__NAMESPACE_START

    enum class COMMAND_TYPE{
        UNKNOWN = -1,
        HELP,       /* Dispays the available commands*/
        RUN,        /* builds and runs the current project */
        FRUN,       /* buils and run the specified source file */
        BUILD,      /* builds the current project */

        /* this can be implicit */
        COMPILE,     /* compiles a single file provided from the argumnets */
        
        COUNT   /* Internally used */
    };

    const char commands[static_cast<u64>(COMMAND_TYPE::COUNT)][8] = {
        "help",
        "run",
        "frun",
        "build",
        "comp",
    };

    /* callbacks */
    typedef Result (*__handler)(int argc, char** argv);

    // predefined handlers
    Result comp_handler(int argc, char** argv) noexcept;
    _NORETURN Result helper(int argc, char** argv) noexcept;
    Result frun_handler(int argc, char** argv) noexcept;

    typedef struct handler{
    public:
        static __handler build;
        static __handler comp;
        static __handler run;
        static __handler frun;
        static __handler help;
    }handler;

    /* this function calls the relative callback 
        function based on the command */
    Result __args(int argc, char** argv) noexcept;

__NAMESPACE_END

#endif

#if defined(BIL__HANDLER_IMPL) && !defined(HANDLER_IMPLEMENTATION)
#define HANDLER_IMPLEMENTATION

__NAMESPACE_START

__handler handler::build =  nullptr;
__handler handler::comp =   comp_handler;
__handler handler::run =    nullptr;
__handler handler::help =   helper;
__handler handler::frun =   frun_handler;

#define UNIMPLEMENTED_COMM_ERR_LOG(_com)\
        do{\
            log::err << "The command: " << _com << " is not implemented yet!" << log::endl;\
            std::exit(1);\
        }while(0)

#define SC_U64(...)         static_cast<u64>(__VA_ARGS__)
#define SELECT_COM(_enum)   commands[SC_U64(_enum)]

#define FAST_FAILURE(_func)\
        if((_func)) std::exit(1);


/////////////////////////////////////////////////////////////
//                      COMP_HANDLER                      //
///////////////////////////////////////////////////////////

Result comp_handler(int argc, char** argv) noexcept{
    assert(argc > 1 && "Expected a filename or a command!");

    std::string_view file(argv[2]);
    core::sh cmd;

    /// The function from the string_view library 
    /// 'ends_with' are only from c++17
#if __cplusplus > 201703L
    if(file.ends_with(".c"))
        cmd << "{CC}";
    else if(file.ends_with(".cpp"))
        cmd << "{CXX}";
#else
    if(std::strncmp(file.end() - 2, ".c", 2) == 0)
        cmd << "{CC}";
    else if(std::strncmp(file.end() - 4, ".cpp", 4) == 0)
        cmd << "{CXX}";  
#endif

    else{
        log::err << "Unknown file: " << file.data() << log::endl;
        std::exit(1);
    }

    cmd << file.data() << "-Wall" << "-ggdb" << core::exec;
    Ok();
}



//////////////////////////////////////////////////////////
//                       HELPER                         //
/////////////////////////////////////////////////////////

_NORETURN Result helper(int argc, char **argv) noexcept{
    std::cout << "Current Available Commands: " << std::endl;
    std::cout << "  -help                   prints this and exit" << std::endl;

    if(handler::run != nullptr)
        std::cout << "  -run {filename}         it build and runs the file" << std::endl;



    if(handler::comp != nullptr)
        std::cout << "  -compile                It compiles a file" << std::endl;
    
    if(handler::build != nullptr)
        std::cout << "  -[build]                It builds the files specified in the main method" << std::endl;
    
    std::exit(0);
}


/////////////////////////////////////////////////////////////
//                      FRUN_HANDLER                      //
///////////////////////////////////////////////////////////

Result frun_handler(int argc, char **argv) noexcept{
    assert(std::strcmp(argv[1], SELECT_COM(COMMAND_TYPE::FRUN)) == 0);

    if(argc < 3){
        log::err << "Expected a filename!" << log::endl;
        std::exit(1);
    }

    std::string file = argv[2];
    core::sh cmd;
    
    cmd << (std::string("./") + std::string(argv[0])) << "comp" << file;

    types::Proc p = cmd.run(true);
    if(p == types::INVALID_PROC)
        std::exit(1);
    
    types::Pecode pc;
    if(core::wait_pecode(p, pc)) std::exit(1);

    if(pc != 0) std::exit(1);

    cmd.reset();
    cmd << "./a.out" << core::exec;

    std::exit(0);
}


//////////////////////////////////////////////////////////
//                      __ARGS                         //
////////////////////////////////////////////////////////

Result __args(int argc, char **argv) noexcept{
    if(argc == 1) Ok( )

    std::string str_com =           argv[1];

    // help command
    if(str_com == SELECT_COM(COMMAND_TYPE::HELP)){
        if((handler::help) != nullptr){
            FAST_FAILURE(handler::help(argc, argv));
        }
        else    UNIMPLEMENTED_COMM_ERR_LOG(str_com);
    }
    
    // run command
    else if(str_com == SELECT_COM(COMMAND_TYPE::RUN)){
        if((handler::run) != nullptr){
            FAST_FAILURE(handler::run(argc, argv));
        }
        else    UNIMPLEMENTED_COMM_ERR_LOG(str_com);
    }
    
    // build command
    else if(str_com == SELECT_COM(COMMAND_TYPE::BUILD)) {
        if((handler::build) != nullptr){
            FAST_FAILURE(handler::build(argc, argv));
        }
        else    UNIMPLEMENTED_COMM_ERR_LOG(str_com);
    }
    
    // compile command
    else if(str_com == SELECT_COM(COMMAND_TYPE::COMPILE)) {
        if((handler::comp) != nullptr){
            FAST_FAILURE(handler::comp(argc, argv));
        }
        else    UNIMPLEMENTED_COMM_ERR_LOG(str_com);
    }

    else if(str_com == SELECT_COM(COMMAND_TYPE::FRUN)){
        if((handler::frun) != nullptr){
            FAST_FAILURE(handler::frun(argc, argv));
        }
        else    UNIMPLEMENTED_COMM_ERR_LOG(str_com);
    }

    Ok();
}

__NAMESPACE_END


#endif
