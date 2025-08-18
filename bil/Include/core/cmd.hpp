#ifndef _CMD_HPP
#define _CMD_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

__NAMESPACE_START
namespace core{

    class sh;
    typedef void (*__f)(sh&);


    /**
     *  This class help menage a shell
     *  with stack variable and running commands
     * **/
    class sh{
    protected:
        using pstat = types::status;
        using pcode = types::Pecode;
        using stat_ref = const types::status&;
        using code_ref = const types::Pecode&;

    public:
        inline sh(void)
            :m_items({}), m_proc(types::INVALID_PROC), 
                m_exitcode(types::INVALID_EXITCODE), 
                m_stat(types::status::UNKNOWN){ }

        inline sh& operator<<(string_ref item){
            return _M_Insert(item);
        }

        sh& operator<<(const std::vector<CStr>& a){
            for(auto& i: a)
                _M_Insert(i);
            return *this;
        }

        inline void operator<<(__f f){
            f(*this);
        }

        inline void print(void) const noexcept{
            log::info << "CMD: " << m_items << log::endl;
        }

        _CONSTEXPR inline auto items(void) const noexcept{
            return this->m_items;
        }

        inline void reset(void) noexcept{
            this->m_items.clear();
        }

        /// @brief Waits that the current stored process 
        ///         while it end its execution 
        /// @return Fails :: INT32_MIN
        types::Pecode wait(void) noexcept;

        /// @brief Waits that the current stored process 
        ///         while it end its execution 
        /// @return Fails :: INT32_MIN
        Result wait(types::Pecode& pe) noexcept;

        /// @brief Checks if the the exit code or the current status are Ok
        /// @return True if something isn't Ok
        inline _CONSTEXPR bool bad(void) const noexcept{
            return this->m_exitcode != 0 || 
                    this->m_stat == pstat::UNKNOWN;
        }
 
        /// @return Return the current process status
        inline _CONSTEXPR stat_ref stat(void) const noexcept{
            return this->m_stat;
        }
 
        /// @return Returns the last process exit code
        inline _CONSTEXPR code_ref code(void) const noexcept{
            return this->m_exitcode;
        }

        /// @brief Runs sync the current stored shell command
        ///         If the command was started in sync  
        /// @return Fails :: INVALID_PROC
        inline types::Proc run(bool reset = false) noexcept {
            types::Proc proc = this->_M_Run(false);
            if(reset && proc != types::INVALID_PROC) this->reset( );
            return this->m_proc;
        }
        
        /// @brief Runs sync the current stored shell command
        ///         If the command was started in sync  
        /// @return Fails :: INVALID_PROC otherwise the proc id
        inline types::Proc run_async(bool reset = false) noexcept{
            types::Proc proc = this->_M_Run(true);
            if(reset && proc != types::INVALID_PROC) this->reset();
            return this->m_proc;
        }

        // TODO[[[[]]]] function that runs and summerize the execution info:
        //      like the execution time the exit code the program the start and end time
        //      the thread id etc...

        /// @brief Runs async or sync the current stored shell command
        ///         If the command was started in sync  
        /// @return Fails :: INVALID_PROC
        ///
        ///         0 => if async = false otherwise the Proc id
        inline types::Proc run(pcode& pe, bool reset = false) noexcept{
            types::Proc proc = this->_M_Run(false);
            if(reset && proc != types::INVALID_PROC) this->reset();
            pe = this->code( );
            return this->m_proc;
        }

    protected:
        sh& _M_Insert(const std::string& item) noexcept;
        
        /* if an item of the vector contains a 
            space it needs to be splited */
        Result _M_SplitItems(void) noexcept;

        types::Proc _M_Run(bool async = false) noexcept;

    private:
        std::vector<std::string> m_items;
        types::Proc m_proc;
        pcode m_exitcode; // Describes the last command exit code
        pstat m_stat;
    };

    // This function call the shell run function in sync mode
    inline void exec(sh& shell){
        (void)shell.run();
    }

    // Exec and Reset the shell
    inline void execeset(sh& shell){
        (void)shell.run(true);
    }
    
}// namespace core
__NAMESPACE_END
#endif

#if defined(BIL__CMD_IMPL) && !defined(CMD_IMPLEMENTATION)
#define CMD_IMPLEMENTATION

__NAMESPACE_START

/////////////////////////////////////////////////////////////
//                     SH::_M_INSERT                      //
///////////////////////////////////////////////////////////
core::sh& core::sh::_M_Insert(string_ref item) noexcept{    
    if(vtable::should(item)){
        std::string __r(item);
        if(!core::vtable::eval(item, __r))
            m_items.push_back(__r);
    }else m_items.push_back(item); 
 
    return *this;
}


/////////////////////////////////////////////////////////////
//                   SH::_M_SPLITITEMS                    //
///////////////////////////////////////////////////////////
Result core::sh::_M_SplitItems(void) noexcept{
    for(size_t i = 0; i < m_items.size(); ++i){
        std::string& __i = m_items[i];
        
        if(__i.find(' ') != std::string::npos){
            if(algor::clean(__i, '\n')) Err( )

            /* Splitting the string into multiple substrings 
                and later appending theme into the m_items vector */
            std::vector<std::string> __t = algor::split_string_to_list(__i);
            
            __try{
                __i = std::move(__t[0]);

                /* Does it throw an excepxtion? */
                m_items.insert((m_items.begin() + i + 1),
                                std::make_move_iterator(__t.begin() + 1),
                                std::make_move_iterator(__t.end()));
            }catch(...){
                log::err << "Unkown Error!" << log::endl;
                Err()
            }
        }
    }

    // log::info << "Result: " << m_items << log::endl;
    Ok( );
}


/////////////////////////////////////////////////////////////
//                        SH::WAIT                        //
///////////////////////////////////////////////////////////
types::Pecode core::sh::wait(void) noexcept{
    if(this->m_proc == types::INVALID_PROC || this->m_proc == 0)
        return types::INVALID_EXITCODE;
    
    if(core::wait_pecode(this->m_proc, this->m_exitcode))
        return types::INVALID_EXITCODE;
    
    return this->m_exitcode;
}


/////////////////////////////////////////////////////////////
//                        SH::WAIT                        //
///////////////////////////////////////////////////////////
Result core::sh::wait(types::Pecode &pe) noexcept {
    if(this->m_proc == types::INVALID_PROC || this->m_proc == 0)
        Err( )
    
    if(core::wait_pecode(this->m_proc, this->m_exitcode))
        Err( )
    
    pe = this->m_exitcode;

    Ok( )
}


/////////////////////////////////////////////////////////////
//                       SH::_M_RUN                       //
///////////////////////////////////////////////////////////
types::Proc core::sh::_M_Run(bool async) noexcept{
    if(this->m_items.empty()){
        log::err << "Connot run an empty command!" << log::endl;
        return types::INVALID_PROC;
    }

    if(this->_M_SplitItems( )) return types::INVALID_PROC;

    this->m_proc = 0;

    if(async){
        this->m_proc = core::create_proc_async(this->m_items);
        this->m_stat = pstat::RUNNING;
    }else{
        if(core::create_proc_sync(this->m_items, this->m_exitcode))
            this->m_stat = pstat::UNKNOWN;
        else
            this->m_stat = pstat::FINISHED;
    }

    return this->m_proc;       
}



__NAMESPACE_END

#endif
