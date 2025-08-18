#ifndef _RESULT_HPP
#define _RESULT_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

#if defined(__BIL_DBG_ST_TRACE__)
#   define BACKWARD_HAS_DW 1
#   include "../debug/backward.hpp"
#endif

__NAMESPACE_START

class __res_impl: public std::error_code {
public:
    inline __res_impl(void)
        :std::error_code({}){ }

    inline __res_impl(const std::error_code& err)
        :std::error_code(err){ }

    inline __res_impl(const __res_impl& err)
        :std::error_code(err){ }
    
    inline __res_impl(const __res_impl&& err)
        :std::error_code(err){ }

    // This avoid the abort call
    inline std::error_code unwrap(void) noexcept{
#if defined(RESULT_FAST_FAILURE)
        return std::move(*this);
#else
        std::error_code __ret(this->value(), this->category());
        clear();
        return __ret;
#endif
    }

    inline bool is_ok(void) const noexcept{
        return (this->value() == 0);
    }

    inline operator bool(void) noexcept{
        return (this->unwrap().value() != 0);
    }

    inline __res_impl& operator=(__res_impl&& imp){
        this->assign(imp.value(), imp.category()); 
        imp.clear();
        return *this;
    }

    inline __res_impl& operator=(__res_impl& imp){
        this->assign(imp.value(), imp.category()); return *this;
    }

    ~__res_impl(){
        if(!is_ok()){
            log::err << "Unhandled Error!";

    #if defined(__BIL_DBG_ST_TRACE__)
            {
                /* Warning two level of function calls are 
                    skippe becouse they refer to the backward library
                    modified function: load_here */
                backward::StackTrace st;
                backward::Printer _printer{false, false, false};
                std::string __res;
                st.load_here( );    /* <--| */
                _printer.print_into_string(st, __res);
                if(__res.empty()) return;
                log::err << " StackTrace: \n" << __res << log::endl;
            }
    #endif
            log::err << " ::Exiting(1)" << log::endl;

            std::exit(1);
        }
    }
};

using Result = __res_impl;

static inline Result make_result(void) noexcept{
    return {};
}

static inline Result make_result(const std::error_code& err) noexcept{
    return {err};
}

// It is consider an errno error
static inline Result make_result(const int& err) noexcept{
    return {{err, std::generic_category()}};
}

/* basic macros for returning an error or an ok state */
#define Ok(...)     return bil::make_result( );
#define Err(...)    return bil::make_result(1);

__NAMESPACE_END

#endif