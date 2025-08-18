#ifndef __VTABLE_HPP
#define __VTABLE_HPP

#include <unordered_map>

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

#define _LOG_FUNC_PRM1(_prm)\
        log::info << "vtable::" << __func__ << "('"\
                  << _prm << "', ..)" << log::endl
#define _LOG_FUNC_PRM2(_prm, _prm2)\
        log::info << "vtable::" << __func__ << "('"\
                  <<_prm << "','" << _prm2 << ", ..)" << log::endl

__NAMESPACE_START

namespace core{

    class __table_imp{
    public:

        /// @brief It adds or reassign the value of a variable in the table
        /// @param name Variable name
        /// @param val Variable value can anything that can be 
        ///             converted to a c++ string.
        /// @warning If the value of the variable is Interpolated this function its only resposible
        ///          to store the Interpolation it will be later Evalutes to the desired value
        static inline void save(string_ref name, string_ref val){
            table.insert_or_assign(name, val);
        }

        /// @brief Its like the other one but this can throw exception
        /// @return The value of the specified variable name.
        /// @throw std::out_of_range if the var name doesn't exist
        static inline std::string& select(string_ref name){
            log::info << "Getting variable: " << name << " value" << log::endl;
            return table.at(name);
        }

        /// @brief This function sets the {val} parameter with the value of the given variable {name}
        ///         If the value is Interpolated this function is not resposible of Evaluate the 
        ///         variable value!
        /// @return Fails :: 1
        static inline Result select(string_ref name, std::string& val) noexcept{
            log::info << log::indt << "Getting variable: '" << name 
                        << "' value" << log::endl;

            try{
                val = table.at(name);
                Ok();
            }catch(std::out_of_range& e){
                log::err << "Syntax Error: Unkown variable named '"
                        << name << "'" << log::endl;
                Err()
            }

            log::info.deindent();
        }

    private:
        static std::unordered_map<std::string, std::string> table;
    };

    class vtable: public __table_imp{
    public:

        ///                     -*- INTERNAL -*-
        /// @brief This function records some predefined variable like:
        ///         - CC defualt C compiler it depends on the platform and 
        ///             the compiler used for compiling the bilder
        ///         - CXX defualt C++ compiler it depends on the platform and 
        ///             the compiler used for compiling the bilder
        static void init(void) noexcept;

        /// @brief This function takes a format that can be a variable name, 
        ///        or an Interpolation and set {res} as its result.
        /// @example eval("{var}", result) //OK;
        ///          eval("var", result) //IGNORED result = "var";
        ///          eval("[command]", result) //OK;
        /// @param rec if set it recurservely evaluate the result if necessary
        /// @return Fails :: 1
        static Result eval(string_ref fmt, std::string& res, bool rec = true) noexcept;

        /// @brief It adds or reassign the value of a variable in the table
        /// @param name Variable name
        /// @param val Variable value can anything that can be 
        ///             converted to a c++ string. It also supports Variables Interpolation {..},
        ///             Commands Interpolation [..].
        static inline Error record(string_ref name, string_ref val){
            log::info << log::indt
                      << "Saving variable: [" << name 
                      << "] = [" << val << "]" << log::endl;
            
            std::string __r(val);

            /* This function accept variable name as expresion to 
                be evaluated so we need to evaluate it if needed */
            if(_M_ShEval(val)){
                if(eval(val, __r)) return _err;
            }

            __table_imp::save(name, __r);

            log::info.deindent();
            return _ok;
        }

        /// @return true if fmt should be evauluated
        _NODISCARD static inline bool should(string_ref fmt){
            return _M_ShEval(fmt);
        }

    private:
        _NODISCARD 
        static _CONSTEXPR inline bool _M_ShEval(string_ref item) noexcept{
            if(item.size() > 2){
                if(
                    (item[0] == '[' || item[0] == '{') 
                        && 
                    (   item[(item.length() - 1)] == '}' || 
                        item[(item.length() - 1)] == ']') 
                ) return true;
            }
            return false;
        }

        _NODISCARD
        static inline Result _M__ErrInvInt(string_ref var) noexcept{
            log::err << "Syntax Error: Empty -> '" 
                << var << "' Expected variable name" << log::endl; 
            Err()
        }
    };
}

__NAMESPACE_END

#endif

#if defined(BIL__VTABLE_IMPL) && !defined(VTABLE_IMPLEMENTATION)
#define VTABLE_IMPLEMENTATION

__NAMESPACE_START

std::unordered_map<std::string, std::string> core::__table_imp::table{};

void core::vtable::init(void) noexcept{
    if(vtable::record("CC", BIL_C_COMPILER)) std::exit(1);
    if(vtable::record("CXX", BIL_CXX_COMPILER)) std::exit(1);
}

Result core::vtable::eval(string_ref fmt, std::string &res, bool rec) noexcept{
    using vt = bil::core::vtable;

    log::info << log::indt << "Evaluating expr: " << fmt << log::endl;

    if(fmt.empty()){
        log::err << "Syntax Error: Invalid fmt passed: (NULL)" << log::endl;
        Err()
    }

    /* it is used to store the fmt
        without the delimeters */
    std::string var;

    if(vt::_M_ShEval(fmt)){

        if(fmt.length() < 3)
            return _M__ErrInvInt(fmt);

        /* This function throw an exception only if _pos {1} 
            is out of range but we already check if 1 is out 
            of range so this cannot happen */
        var.assign(fmt, 1, fmt.length() - 2);

        // format evaluator
        if(fmt[0] == '{'){
            if(select(var, res)) 
                Err( );   /* getting the variable value */

            /* recursive call */
            if(rec && vt::_M_ShEval(res)){
                // log::info.deindent();
                if(vt::eval(res, var, rec)) Err( );
            }

        }else{
            // getting the argument list from the fmt 
            types::Args __a = algor::split_string_to_list(var);

            res = core::shout_stdout(__a);  /* fails if res.empty() = true */
            if(res.empty()) Err()
        }
        
    }else   res = fmt;  /* Ignoring */

    if(res.back() == '\n'){
        res.pop_back( );
    }
    
    log::info << "Evaluated result: " << res 
            << log::endl;
    
    log::info.deindent();
    Ok()
}

__NAMESPACE_END

#endif
