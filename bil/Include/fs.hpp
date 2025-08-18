#ifndef __FS_HPP
#define __FS_HPP
#if __cplusplus >= 201703L

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

__NAMESPACE_START
namespace fs{
    namespace fsys =    std::filesystem;

    using path =        fsys::path;
    using path_ref =    const path&;
    using paths =       std::vector<path>;
    using fstatus =     fsys::file_status;
    using ftype =       fsys::file_type;
    using open_mode =   std::ios_base::openmode;
    using ftime =       fsys::file_time_type;

    class temp_path;
    
    static constexpr inline auto OPEN_MODE = std::ios::in | std::ios::binary;
    static constexpr inline auto ZERO_TIME = ftime();

    /// @param path The path to check
    /// @return Fails :: 1
    fstatus file_status(path_ref path) noexcept;

    _NODISCARD inline ftime file_time(path_ref  path) noexcept{
        std::error_code code;
        log::info << "Getting file time of: " << path << log::endl;
        ftime __r = fsys::last_write_time(path, code);
        if(code){
            log::err << "Unable to get the file '" << path << "' last write time: " 
                     << code.message() << log::endl;
            return fs::ZERO_TIME;
        }
        return __r;
    }

    _NODISCARD inline ftype file_type(path_ref  path) noexcept{
        return fs::file_status(path).type();
    }

    /// @return The current working dir or an empty path if an error occured 
    _NODISCARD inline path curdir(void) noexcept{
        std::error_code err;
        path __p{fsys::current_path(err)};
        if(err)
            return {};
        else return __p;
    }

    /// @brief Checks if the file specifed exist
    /// @return Exist :: true] [Fails :: false
    bool file_exist(path_ref  path) noexcept;

    /// @return True if is a dir otherwise false (even for errors)
    _NODISCARD inline bool is_dir(path_ref  path) noexcept{
        return fs::file_type(path) == ftype::directory;
    }

    /// @return Comparator enum indicating the relation between f1 and f2 file time
    ///
    ///         - Comparator::MORE  =>  f1 is newer than f2
    ///
    ///         - Comparator::EQUAL =>  f1 == f2
    ///
    ///         - Comparator::LESS  =>  f1 is older than f2
    _NODISCARD Comparator cmp_filetime(path_ref f1, path_ref f2) noexcept;

    /// @return True :: if f1 and f2 filetime are equal
    _NODISCARD inline bool ft_equ(path_ref f1, path_ref f2) noexcept{
        return cmp_filetime(f1, f2) == Comparator::EQUAL;
    }

    /// @return True :: if f1 is newer than f2
    _NODISCARD inline bool ft_newer(path_ref f1, path_ref f2) noexcept{
        return cmp_filetime(f1, f2) == Comparator::MORE;
    }

    /// @return True :: if f1 is older than f2
    _NODISCARD inline bool ft_older(path_ref f1, path_ref f2) noexcept{
        return cmp_filetime(f1, f2) == Comparator::LESS;
    }

    /// @brief returns the filename of the path. example:
    ///         path/to/som/file.txt => file.txt its returned
    _NODISCARD path base_name(path_ref path) noexcept {
        return std::filesystem::path(path).filename();
    }

    /// @return Fails :: 1
    Result mkdir_if_nexists(path_ref path) noexcept;

    /// @return Fails :: 1 
    Result mkdir_if_nexists_rec(path_ref path) noexcept;

    /// @return Fails :: 1
    Result mkdir(path_ref path) noexcept;

    /// @brief The file or empty directory identified by the path 
    ///         is deleted as if by the POSIX remove. Symlinks are not followed 
    ///         (symlink is removed, not its target).
    /// @return Fails :: 1
    Result remove(path_ref path) noexcept;

    /// @brief Deletes the contents of {path} (if it is a directory) and the 
    ///         contents of all its subdirectories, recursively, then deletes {path}
    ///         itself as if by repeatedly applying the POSIX remove.
    ///         Symlinks are not followed (symlink is removed, not its target).
    /// @return Fails :: 1
    Result remove_all(path_ref path) noexcept;

    /// @brief Renames the {old} to {new_file}
    /// @return Fails :: 1
    Result rename(path_ref old, path_ref new_file) noexcept;

    /// @brief Creates a file from the path
    /// @warning If path already exist it will be truncated,
    ///          to avoid this consider using the 'create_file_if_nexist' func 
    /// @return Fails :: 1
    Result create_file(path_ref path) noexcept;

    /// @return Fails :: empty()
    _NODISCARD std::string read_entire_file(path_ref path, const open_mode mode = OPEN_MODE) noexcept;
    
    /// @brief Create a file only if it doesn't exist
    /// @return Fails :: 1
    Result create_file_if_nexist(path_ref path) noexcept;

    /// @brief It sets the path equal to the system/user temp directory path
    /// @return Fails :: 1
    Result get_temp_dir(fs::path& path) noexcept;

    /// @brief It sets the path equal to the system/user temp directory path
    /// @return Fails :: 1
    Result get_temp_dir(std::string& path) noexcept;

    /// @brief It creates a temporary file name and sets it in the path parameter
    ///         The file name is located in the system temp folder obtained from the
    ///         get_temp_dir function. 
    /// @return Fails :: 1
    Result tmp_fname(fs::path& path) noexcept;

    /// @brief This function reads a directory and returns 
    /// @param filter The filter to use to remove from the returned files
    ///              Its a basic filtering so it just accept one * like the following ones:
    ///              *.txt => This filter is used to get only the files that ends with the .txt extension
    ///              test* => This filter is used to get only the files that starts with 'test'
    ///              *test* => This filter is used to get only the files that contains the word 'test' 
    /// @return A list of files
    _NODISCARD paths read_dir(path_ref dir, string_ref filter, bool rec = true) noexcept;    

    /// @brief This function reads a directory and returns the content of "dir"
    /// @return The list of files
    _NODISCARD paths read_dir(path_ref dir, bool rec = true) noexcept;

    /**
     *  BASIC TEMP_PATH CLASS THAT DESTROYS THE FILE
     *  AUTOMATICALLY WHEN THE CLASS IS DESTROYED
     * */
    class temp_path: public fsys::path{
        public:
            explicit inline temp_path(void){
                if(_M_GenFile( )) std::abort( );   
            }

            _NODISCARD inline std::string read_entire_file(void) const noexcept{
                return fs::read_entire_file(*this);
            }

            inline ~temp_path(){
                if(!empty( )) fs::remove(*this);
            }

        private:
            inline Result _M_GenFile(void) noexcept{
                return fs::tmp_fname(*this);
            } 
    };
}

__NAMESPACE_END
#else
#   error "Please use a newer C++ standard! Required at least: C++17"
#endif

// #if 1
#if defined(BIL__FILES_IMPL) && !defined(FILES_IMPLEMENTATION) 
#   define FILES_IMPLEMENTATION
__NAMESPACE_START

Result bil::fs::mkdir(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    std::error_code err;
    log::info << "Creating Dir: " << path << log::endl;

    if(!fsys::create_directory(path, err)){
        log::err << "It was not possible to create the dir: " 
                 << path << " :: => " << strerror(err.value())
                 << log::endl;
        Err( )
    }

    Ok( )
}

fs::fstatus fs::file_status(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    std::error_code err;

    fs::fstatus __stat = fsys::status(path, err);

    if(__stat.type() == fs::ftype::none){
        log::err << "Path " << path
                    << ": " << err.message()
                    << log::endl;
    }

    return __stat;
}

bool fs::file_exist(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    std::error_code err;

    if(fsys::exists(path, err)){
        return true;
    }else if(err.value() != 0){
        log::err << "Could not determinate if the file " 
                 << path << " exist: " 
                 << err.message() << log::endl;
    }

    return false;
}

Comparator fs::cmp_filetime(path_ref f1, path_ref f2) noexcept{
    assert(!(f1.empty() || f2.empty()) && "Empty path Error");

    ftime t1 = file_time(f1);
    ftime t2 = file_time(f2);
    
    if(t1 == fs::ZERO_TIME || t2 == fs::ZERO_TIME) return Comparator::UNKNOWN;

    if(t1 == t2)    return Comparator::EQUAL;
    if(t1 > t2)     return Comparator::MORE;

    return Comparator::LESS;
}

Result fs::mkdir_if_nexists(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    if(!fs::file_exist(path)){
        return fs::mkdir(path);
    }else
        log::warn << "Cannot create directory "
                  << path << ": Already exist"
                  << log::endl;

    Ok( )
}

Result fs::mkdir_if_nexists_rec(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    if(!fs::file_exist(path) && !path.empty()){
        fs::path __p = path.parent_path();
        if(!__p.empty() && fs::mkdir_if_nexists_rec(__p))
            Err( )

        return fs::mkdir(path);
    }

    Ok( )
}

Result fs::remove(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    std::error_code err;

    if(!fs::file_exist(path)){
        log::warn << "File " << path << " doesn't exist" << log::endl;
        Err( )
    }

    log::info << "Removing file: " << path << log::endl;

    if(!fsys::remove(path, err)){
        log::err << "Cannot remove file " << path
                 << ": " << strerror(err.value())
                 << log::endl;

        Err( )
    }

    Ok( )
}

Result fs::remove_all(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    std::error_code err;

    if(!fs::file_exist(path)){
        log::warn << "File " << path << " doesn't exist" << log::endl;
        Err( )
    }

    for(auto& f: fsys::directory_iterator(path)){
        path_ref p = f.path();

        if(fs::is_dir(p)){
            fs::remove_all(p);
        }else if(fs::remove(p)) Err( )
    }

    return fs::remove(path);
}

Result fs::create_file(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    if(fs::file_exist(path))
        log::warn << "Path " << path 
                  << ": Already exists" << log::endl;
    
    log::info << "Creating file: " 
              << path << log::endl;
    
    std::ofstream f(path);
    if(f.bad()){
        log::err << "Unable to open file " << path << ": " << strerror(errno) << log::endl;
        Err( )
    }
    
    f.close();
    Ok( )
}

Result fs::create_file_if_nexist(path_ref path) noexcept{
    assert(!path.empty() && "Empty path Error");

    if(fs::file_exist(path)){
        log::info << "Path " << path << " Already exist" << log::endl;
        Ok( )
    }

    log::info << "Creating file: " << path << log::endl;
    std::ofstream f(path);

    if(f.bad()) Err( )

    Ok( )
}


fs::paths fs::read_dir(path_ref dir, string_ref avoid, bool rec) noexcept{
    assert(!dir.empty() && "Empty path error");

    paths ret;
    
    /* 0 = default 
       1 = ends with
       2 = starts with 
       3 = contains
    */
    int type = 0;
    size_t idx;

    if((idx = avoid.find('*')) != std::string::npos){
        if(idx == 0)
            type = 1;   /* ends with */
        else if(idx == avoid.length())
            type = 2;   /* starts with */
        else
            type = 3;   /* contains */
    }

    for(const auto& f: fsys::directory_iterator(dir)){
        if((rec) && (f.is_directory())){
            fs::paths __r = fs::read_dir(f, avoid);
            ret.insert( ret.end(), 
                        std::make_move_iterator(__r.begin()),
                        std::make_move_iterator(__r.end()));
        }

        if(type == 1){
            string_ref p = f.path().c_str();
            if(!p.ends_with(&avoid[1])) continue;
        }else if(type == 2){
            string_ref p = f.path().c_str();
            if(!p.starts_with(std::string_view(p.c_str(), p.length() - 1))) continue;
        }else if(type == 3){
            string_ref p = f.path().c_str();
            if(p.find(&avoid[1], 0, (avoid.length() - 1)) == std::string::npos) continue;
        }else{
            string_ref p = f.path().c_str();
            if(p != avoid) continue;
        }

        ret.push_back(f);
    }

    return ret;
}

fs::paths fs::read_dir(path_ref dir, bool rec) noexcept{
    assert(!dir.empty() && "Empty path error");
    
    log::info << "Reading dir: " << dir.c_str() << log::endl;

    fs::paths ret{ };
    try{
        for(auto& f: fsys::directory_iterator(dir)){
            if((rec) && (f.is_directory())){
                // string_ref __p = f.path().c_str();
    
                // if(__p != "." && __p != ".."){
                fs::paths __r = fs::read_dir(f);
                ret.insert( ret.end(), 
                            std::make_move_iterator(__r.begin()),
                            std::make_move_iterator(__r.end()));
                // }
            }
            
            ret.push_back(f);
        }
    }catch(const std::exception& e){
        log::err << "Error while reading the dir: " << e.what() << log::endl;
        return {};
    }

    return ret;
}


Result fs::get_temp_dir(fs::path &path) noexcept{
    std::error_code err;
    
    path = fsys::temp_directory_path(err);
    log::info << "Temp dir: " << path << log::endl;

    if(path.empty()){
        log::err << "Unable to retrive the system temp directory: " 
                 << err.message() << log::endl;
        Err( )
    }

    Ok( )
}

Result fs::get_temp_dir(std::string &path) noexcept{
    std::error_code err;
    
    path = fsys::temp_directory_path(err).string();
    log::info << "Temp dir: " << path << log::endl;

    if(path.empty()){
        log::err << "Unable to retrive the system temp directory: " 
                 << err.message() << log::endl;
        Err( )
    }

    Ok( )
}

Result fs::tmp_fname(path &path) noexcept{
    std::string buf;
    if(get_temp_dir(buf)) Err( )

#if defined(_WIN32)
    buf += "\\XXXXXX";
#else
    buf += "/XXXXXX";
#endif
    if(mkstemp(&buf[0]) < 0){
        log::err << "Unable to create a temporary file name from template: "
                 << buf << " :: => " 
                 << strerror(errno) << log::endl;
        Err( )
    }

    path = std::move(buf);
    log::info << "Temp file name: " << path << log::endl;
    Ok( )
}

std::string fs::read_entire_file(path_ref path, const open_mode mode) noexcept{
    std::stringstream s;
    std::string result{ };
    
    if(!(mode & std::ios::in)){
        log::err << "Inval mode passed!" << log::endl;
        return s.str( );
    }
    std::fstream f(path, mode);

    f.clear( );
    s << f.rdbuf( );

    if(s.fail()){
        if(s.bad()){
            log::err << "Unable to reading from " << path << log::endl;
        }else result[0] = '\0';
        
    }else result = s.str();

    return result;
}


Result fs::rename(path_ref old, path_ref new_file) noexcept{
    std::error_code code;
    if(old.empty() || new_file.empty()){
        log::err << "Invalid path given! F:<" << "fs::rename" << ">" << log::endl;
        Err( )
    }

    log::info << "Ranaming: " << old << " to: " << new_file << log::endl;
    fsys::rename(old, new_file, code);

    if(code){
        log::err << "Unable to rename file " << old << " to " 
                << new_file << ": " << code.message() 
                << log::endl;
        Err( )
    }

    Ok( )
}

__NAMESPACE_END
#endif
#endif
