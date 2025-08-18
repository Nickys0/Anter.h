/**
 *      
 *      HEADER: Include/log.hpp
 *      It define the logging system of the
 *       library.
 *      Class definition: log::basic_log, there are 
 *       three instance of this class: 
 *       log::info, log::warn, log::err
 * 
 *      FUNCTION DECLARATION:
 *      - inline log::basic_log& endl(log::basic_log& e);
 *      - [[noreturn]] void UNIMPLEMENTED(CStr func, int line, CStr file);
 *      - [[noreturn]] void UNREACHABLE(CStr msg, int line, CStr file);
 *   
 **/
#ifndef _DIMLOG_HPP
#define _DIMLOG_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

__NAMESPACE_START

/// @brief 
namespace log{
	class basic_log;
	typedef log::basic_log&(*__f)(log::basic_log&);

	class basic_log{
	public:
		inline basic_log(CStr mask)
            :   m_mask(mask), m_disabled(false), 
                m_indent(-2){ };
		
        _CONSTEXPR inline basic_log(const basic_log& l)     = delete;
        _CONSTEXPR inline basic_log(const basic_log&& l)    = delete;

		log::basic_log& log(CStr end = "\n"){
            if(!m_disabled){
                /* we only print the mask if there is something to print */
                if(this->m_buf.str().size() > 0)
                    std::cout << this->m_mask; 

#if !defined(BIL_NO_DEFAULT_INDT)
                    if(m_indent != 0){
                        for(int i = 0; i < m_indent; ++i){
                            std::cout << ' ';
                        }
                    }
#endif
                    std::cout << this->m_buf.str() << end;
                }

            this->m_buf.str("");
            return *this;
		}

		/// @brief Input operator
		/// @param f function that flushes the log 
		/// @return The current instance of the class
		inline basic_log& operator<<(__f f){
			return f(*this);
		}

        /// @brief If this function is called the curent
        ///         instance of the class cannot print
        ///         in the output stream untill the enable 
        ///         function is called.
        _CONSTEXPR inline void disable(void) noexcept{
            m_disabled = true;
        }
        
        /// @brief If this function is called it enables the
        ///         output stream of the current class instance
        _CONSTEXPR inline void enable(void) noexcept{
            m_disabled = false;
        }

        _CONSTEXPR inline basic_log& indent(void) noexcept{
            m_indent += 2;
            return *this;
        }

        _CONSTEXPR inline basic_log& deindent(void) noexcept{
            m_indent -= (2 * (m_indent > 1));
            return *this;
        }

		template<typename T>
		inline basic_log& operator<<(const T& value){
            return this->_M_Insert(value);
		}

        basic_log& operator<<(const types::Args& value){
            for(auto& i: value){
                this->_M_Insert(i) << " ";
            }
            return *this;
		}

        inline basic_log& operator<<(const char* value){
            return this->_M_Insert(value);
        }

		~basic_log(){
			if(this->m_buf.rdbuf()->in_avail() != 0)
				this->log();
		}

    protected:
        _CONSTEXPR inline auto _M_mask(void)const noexcept{
            return m_mask;
        }

        _CONSTEXPR inline const std::stringstream& _M_buf(void)const noexcept{
            return m_buf;
        }

        template<typename T>
        inline basic_log& _M_Insert(const T& value){
            this->m_buf << value;
            return *this;
        }

	private:
		CStr m_mask;
		std::stringstream m_buf;
        bool m_disabled;
        int m_indent;
	};

    inline log::basic_log& endl(log::basic_log& e){
        return e.log();
    }

    inline log::basic_log& indt(log::basic_log& e){
        return e.indent();
    }

    inline log::basic_log& dindt(log::basic_log& e){
        return e.deindent();
    }

    inline log::basic_log New(CStr mask){
        return basic_log{mask};
    }

    /// @brief It prints an error log indicating  the function the line 
    ///         and the file provided through the parameters
    /// @return !Doesn't return
	_NORETURN
    void UNIMPLEMENTED(CStr func, int line, CStr file);

    /// @brief It prints an error log indicating the msg the line 
    ///         and the file provided through the parameters
    /// @return !Doesn't return
    _NORETURN
	void UNREACHABLE(CStr msg, int line, CStr file);

    extern  log::basic_log info;
    extern  log::basic_log warn;
    extern  log::basic_log err;
}
__NAMESPACE_END

/// @brief It prints an error log indicating the function the line 
///         and the file provided through the parameters
#define Unimpl(...)\
        bil::log::UNIMPLEMENTED(__func__, __LINE__, __FILE__);

/// @brief It prints an error log indicating the msg the line 
///         and the file provided through the parameters
#define Unreach(...)\
        bil::log::UNREACHABLE(__VA_ARGS__, __LINE__, __FILE__)
#endif

#if defined(BIL__LOG_IMPL) && !defined(LOG_IMPLEMENTATION)
#   define LOG_IMPLEMENTATION 1
__NAMESPACE_START

    _NORETURN
	void log::UNIMPLEMENTED(CStr func, int line, CStr file){
		std::cout 	<< "[::" MK_RED("UNIMPLEMENTED") "]: (<" << func << ">)" << "\n"
					<< "                 => FILE: " << file << "\n"
		            << "                 => LINE: " << line << std::endl;
		std::exit(1);
	}

    void log::UNREACHABLE(CStr msg, int line, CStr file){
        std::cout 	<< "[::" MK_RED("UNREACHABLE") "]: "    << "\n"
        << "                 => FILE: " << file << "\n"
        << "                 => LINE: " << line << "\n"
        << msg << std::endl;

        std::abort();
    }

__NAMESPACE_END

bil::log::basic_log bil::log::info      ( "[" MK_HCYAN("INFO")  "] " );
bil::log::basic_log bil::log::warn      ( "[" MK_HYELLOW("WARN")"] " );
bil::log::basic_log bil::log::err       ( "[" MK_BRED("ERROR")  "] " );

#endif
