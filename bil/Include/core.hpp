#ifndef _PROC_HPP
#define _PROC_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

__NAMESPACE_START

namespace core{
    using args = types::Args;
    using args_ref = const types::Args&;

    using proc = types::Proc;
    using proc_ref = const types::Proc&;

    using procs = types::Procs;
    using procs_ref = const types::Procs;

    /*  Structure that contains the file 
        descriptor for redirecting */
    typedef struct __redirect{
    public: 
        Fd fdout =  types::INVALID_FD;
        Fd fdin  =  types::INVALID_FD;
        Fd fderr =  types::INVALID_FD;
    }Fd_Rdr;
    
    /// TODO::
    // Fd open_file(const std::string& path, const Fmode& mode);

    void go_rebuild_yourself(int argc, char** argv, string_ref file);

    //?Should we move all this function into the fs header?
    /// @brief Opens a file for reading from path and returns its fd  
    /// @return The opened file descriptor. Fails :: INVALID_FD
    Fd open_file_for_read(const fs::path& path) noexcept;
    
    /// @brief Opens a file for writing from path and returns its fd  
    /// @return The opened file descriptor. Fails :: INVALID_FD
    Fd open_file_for_write(const fs::path& path) noexcept;
    
    /// @brief Opens a temp file for writing from path and returns its fd
    /// @return The opened file descriptor. Fails :: INVALID_FD
    Fd open_file_for_write(void) noexcept;
    
    /// @brief Closes an opened fd  
    void close_fd(Fd fd) noexcept;
    /************************************************** */



    /// @brief Waits untill all processed have ended their lifetime
    /// @return Fails :: 1
    Result wait(procs_ref procs) noexcept;
    
    /// @brief Waits untill the process has ended its lifetime
    /// @return Fails :: 1
    Result wait(proc_ref proc) noexcept;

    /// @brief Waits untill the process has ended its lifetime and sets
    ///         code with the terminated process exit code
    /// @return Fails :: 1
    Result wait_pecode(proc proc, Pecode& code) noexcept;



    //TODO::
    //? Should I create a single function create_proc that as
    //?  a bool to indicate an async or sync behaviour?
    // Fd create_proc(args_ref cmd, Fd_Rdr = {}, bool async = false);
    // Fd create_proc(args_ref cmd,types::Pecode& pc, Fd_Rdr = {}, bool async = false);

    /// @brief Creates a new async process with argument = cmd
    /// @return Fails :: types::INVALID_PROC
    Proc create_proc_async(args_ref cmd, Fd_Rdr rd = {}) noexcept;

    /// @brief Creates a new sync process with argument = cmd
    /// @return Fails :: 1
    Result create_proc_sync(args_ref cmd, Fd_Rdr rd = {}) noexcept;

    /// @brief Creates a new sync process with argument = cmd and sets 
    ///         the pc parameter with the command exit code
    /// @return Fails :: 1
    Result create_proc_sync(args_ref cmd, types::Pecode& pc, Fd_Rdr rd = {}) noexcept;




    /// @brief Exucute the command in the sh parameter with redirecting stdout
    /// @return Fails :: 1
    Result shout(args_ref sh, std::string& out, std::string& err) noexcept;

    /// @brief Exucute the command in the sh parameter with redirecting stdout
    /// @return Fails :: empty(). If the command doesn't produce any 
    ///         output out[0] = '\0'
    std::string shout_stdout(args_ref sh) noexcept;

    /// @brief Exucute the command in the sh parameter with redirecting stderr
    /// @return Fails :: empty(). If the command doesn't produce any 
    ///         output out[0] = '\0'
    std::string shout_stderr(args_ref sh) noexcept;
}

__NAMESPACE_END

#endif

#if defined(BIL__PROC_IMPL) && !defined(PROC_IMPLEMENTATION)
#define PROC_IMPLEMENTATION

__NAMESPACE_START


///////////////////////////////////////////////////////////
//                  OPEN_FILE_FOR_READ                  //
/////////////////////////////////////////////////////////

Fd core::open_file_for_read(const fs::path& path) noexcept{
#ifdef _WIN32
#   error "WIN32 Platform still not supported with this function"
#else
    Fd result = open(path.string().c_str(), O_RDONLY);
    if(result < 0){
        log::err << "Could not open file " << path << " :" << strerror(errno) << log::endl;
        return types::INVALID_FD;
    }
    return result;
#endif // _WIN32
}


///////////////////////////////////////////////////////////
//                  OPEN_FILE_FOR_WRITE                 //
/////////////////////////////////////////////////////////

Fd core::open_file_for_write(const fs::path& path) noexcept{
    if(path.empty()) return _err;

#ifdef _WIN32
#   error "WIN32 Platform still not supported with this function"
#else
    Fd result = open(path.string().c_str(), O_WRONLY);
    if(result < 0){
        log::err << "Could not open file " << path << " :" << std::strerror(errno) << log::endl;
        return types::INVALID_FD;
    }
    return result;
#endif // _WIN32
}


///////////////////////////////////////////////////////////
//                  OPEN_FILE_FOR_WRITE                 //
/////////////////////////////////////////////////////////

Fd core::open_file_for_write(void) noexcept{
    fs::path __p;
    if(fs::tmp_fname(__p)) return _err;

    return core::open_file_for_write(__p);
}


///////////////////////////////////////////////////////////
//                       CLOSE_FD                       //
/////////////////////////////////////////////////////////

void core::close_fd(Fd fd) noexcept
{
#ifdef _WIN32
    CloseHandle(fd);
#else
    close(fd);
#endif // _WIN32
}


//////////////////////////////////////////////////////////
//                        WAIT                          //
/////////////////////////////////////////////////////////

Result core::wait(procs_ref procs) noexcept{
    Result _ret{};
    
    for(auto& i: procs){
        if(wait(i)) _ret = make_result(1);
    }

    return _ret;
}


//////////////////////////////////////////////////////////
//                    WAIT_PECODE                       //
/////////////////////////////////////////////////////////

Result core::wait_pecode(proc proc, Pecode &code) noexcept{
    if(proc == types::INVALID_PROC){
        log::err << "Invalid Process to wait!" << log::endl;
        Err()
    }

#ifdef _WIN32
#   error "WIN32 PLatform still not supported with this function"
#else
    int wstatus = 0;
    
    if(waitpid(proc, &wstatus, 0) < 0){
        log::err << "Could not wait process (pid " << proc << "): " << strerror(errno) << log::endl;
        Err()
    }

    if(WIFEXITED(wstatus)){
        if((code = WEXITSTATUS(wstatus)) != 0){
            log::info << "Command exited with exit code: " << code << log::endl;
            Err( )
        }
    }

    else if(WIFSIGNALED(wstatus)){
        log::err << "Process terminated by " << strsignal(WTERMSIG(wstatus)) << log::endl;
        Err( )
    }
    
#endif
    Ok()    
}


//////////////////////////////////////////////////////////
//                         WAIT                         //
/////////////////////////////////////////////////////////

Result core::wait(proc_ref proc) noexcept{
    Pecode code = 0;
    return wait_pecode(proc, code);
}


//////////////////////////////////////////////////////////
//                  CREATE_PROC_ASYNC                   //
/////////////////////////////////////////////////////////

Proc core::create_proc_async(args_ref cmd, Fd_Rdr rd) noexcept{
    types::Proc result = types::INVALID_FD;

    if(cmd.empty()){
        log::err << "Empty command is not allowed!" << log::endl;
        return result;
    }

    log::info << log::indt << "EXEC: " << cmd << log::endl;

#ifdef _WIN32
#   error "WIN32 Platform still not supported with this function"
#else
    result = fork();
    if(result < 0){
        log::err << "Could not fork child process: " << strerror(errno) << log::endl;
        return types::INVALID_PROC;
    }

    // This code is executed only in the child process
    if(result == 0){
        if(rd.fdin != types::INVALID_FD){
            if(dup2(rd.fdin, STDIN_FILENO) < 0){
                log::err << "Could not setup stdin for child process: " 
                         << strerror(errno) << log::endl;
                exit(1);
            }
        }
        
        if(rd.fdout != types::INVALID_FD){
            if(dup2(rd.fdout, STDOUT_FILENO) < 0){
                log::err << "Could not setup stdin for child process: " 
                         << strerror(errno) << log::endl;
                exit(1);
            }
        }
        
        if(rd.fderr != types::INVALID_FD){
            if(dup2(rd.fderr, STDERR_FILENO) < 0){
                log::err << "Could not setup stderr for child process: " 
                         << strerror(errno) << log::endl;
                exit(1);
            }
        }

        // NOTE: This leaks a bit of memory in the child process
        char** items = new (std::nothrow) char*[cmd.size() + 1]; 
        assert(items != nullptr && "Unable to allocate");
        
        std::memset(items, 0, ((cmd.size() + 1) * sizeof(char*)));

        // log::info << "Debug::" << log::endl;
        // if(cmd[0] == "c++")
        //     std::cerr << "DEBUG::" << std::endl;


        for(size_t i = 0; i < cmd.size(); ++i){
            char* item = strdup(cmd[i].c_str());
            assert(item != nullptr && "Unable to allocate!");

            // log::info << "ARG[" << i <<"]: " << item << log::endl;
            // if(cmd[0] == "c++")
            //     std::cerr << "ARG[" << i << "]: " << item << std::endl;
            items[i] = item;
        }
        
        // This will leak a bit of memory but who cares
        if(execvp(cmd[0].c_str(), (char* const*)items) < 0){
            log::err << "Could not exec child process: " << strerror(errno) << log::endl; 
            exit(1);
        }
    }

    log::info.deindent( );
    return result;
#endif
}


///////////////////////////////////////////////////////////
//                  CREATE_PROC_SYNC                    //
/////////////////////////////////////////////////////////

Result core::create_proc_sync(args_ref cmd, Fd_Rdr rd) noexcept{
    types::Proc Pr = core::create_proc_async(cmd, rd);
    if(types::INVALID_FD == Pr) Err( )
    return core::wait(Pr);
}


/////////////////////////////////////////////////////////////
//                    CREATE_PROC_SYNC                    //
///////////////////////////////////////////////////////////

Result core::create_proc_sync(args_ref cmd, types::Pecode &pc, Fd_Rdr rd) noexcept{
    types::Proc p = core::create_proc_async(cmd, rd);
    if(types::INVALID_FD == p) Err( )
    return core::wait_pecode(p, pc);
}


///////////////////////////////////////////////////////////
//                        SHOUT                         //
/////////////////////////////////////////////////////////

Result core::shout(args_ref sh, std::string &out, std::string &err) noexcept{
    log::info << "Getting shell output: {stdout} + {stderr}" << log::endl;
    
    /* we don't accept empty commands */
    if(sh.empty()){
        log::err << "Invalid command: " << "(NONE)" << log::endl;
        Err()
    }

    fs::temp_path _out_path{};  /* generatign a random file for stdout redirecting */
    fs::temp_path _err_path{};  /* generating a random file for stderr redirecting */

    /* we ensure empty strings */
    out.clear(); err.clear();

    /* Redirecting output */
    types::Fd __o = core::open_file_for_write(_out_path);
    types::Fd __e = core::open_file_for_write(_err_path);

    core::Fd_Rdr r{
        .fdout = __o,
        .fderr = __e
    };

    /* executing */
    if(!core::create_proc_sync(sh, r)){
        out = _out_path.read_entire_file();
        err = _err_path.read_entire_file();
    }

    /* closing all the unaccessary fd */
    close_fd(__o);
    close_fd(__e);
    Ok( )
}


//////////////////////////////////////////////////////////
//                    SHOUT_STDOUT                     //
////////////////////////////////////////////////////////

std::string core::shout_stdout(args_ref sh) noexcept{
    std::string out{};

    log::info << log::indt << "Getting shell output: {stdout}" << log::endl;

    /* we don't accept empty commands */
    if(sh.empty()){
        log::err << "Invalid command: " << "(NONE)" << log::endl;
        return out;
    }

    /* generatign a random file for stdout redirecting, 
        it is automatically deleted*/
    fs::temp_path _out_path{};
    
    /* Redirecting output */
    Fd __o = core::open_file_for_write(_out_path);
    core::Fd_Rdr r{ .fdout = __o, };

    /* executing */
    if(!core::create_proc_sync(sh, r))
        out = _out_path.read_entire_file();

    /* closing all the unaccessary fd */
    close_fd(__o);

    log::info.deindent();
    return out;
}


//////////////////////////////////////////////////////////
//                    SHOUT_STDERR                     //
////////////////////////////////////////////////////////

std::string core::shout_stderr(args_ref sh) noexcept{
    std::string out{};

    log::info << "Getting shell output: {stderr}" << log::endl;

    /* we don't accept empty commands */
    if(sh.empty()){
        log::err << "Invalid command: " << "(NONE)" << log::endl;
        return out;
    }

    /* generatign a random file for stdout redirecting */
    fs::temp_path _out{};
    
    /* Redirecting output */
    Fd __o = core::open_file_for_write(_out);
    core::Fd_Rdr r{ .fderr = __o, };

    /* executing */
    if(!core::create_proc_sync(sh, r))
        out = std::move(_out.read_entire_file());

    /* closing all the unaccessary fd */
    close_fd(__o);
    return out;
}


//////////////////////////////////////////////////////////
//                 GO_REBUILD_YOURSELF                 //
////////////////////////////////////////////////////////

void core::go_rebuild_yourself(int argc, char **argv, string_ref file){
    std::string binary_path(argv[0]);
    log::info << "Binary path " << binary_path << log::endl;

    /* We first need to know if we should recompile the file, 
        so we check the two file times*/
    Comparator __res = fs::cmp_filetime(file, binary_path);

    if(__res == Comparator::UNKNOWN) std::exit(1);  /* error */
    if(__res != Comparator::MORE) return;           /* The binary is newer */
    
    log::info << "Rebuilding file" << log::endl;

#if defined(BIL_SYSTEM_LINUX)
    types::Args __a = { "c++", "-o", binary_path, file, "-ggdb", "-std=c++20", "-Wall", 
#   if defined(__BIL_DBG_ST_TRACE__)
        "-ldw" 
#   endif
    };
#else
#   error "Platform not supported yet!"
#endif

    /* We rename the current binary into binary.old 
        so if an error occured while recompaling we can
        come back */
    std::string old_binary(binary_path + ".old");
    
    if(fs::rename(binary_path, old_binary)) 
        std::exit(1);


    if(core::create_proc_sync(__a)){
        fs::rename(old_binary, binary_path).unwrap();
        std::exit(1);
    }
    
    if(fs::remove(old_binary)) 
        std::exit(1);

    __a.clear();

    for(int i = 0; i < argc; i++){
        __a.push_back(argv[i]);
    }
    
    log::info << "------------ -*- STARTING BIL -*- ------------" << log::endl;
    if(core::create_proc_sync(__a)) 
        std::exit(1);

    exit(0);
}

__NAMESPACE_END


#endif

