#ifndef _ALGOR_HPP
#define _ALGOR_HPP

#ifndef _BIL_HPP
#   error "You should not Include this header alone! Include bil.hpp instead"
#endif

__NAMESPACE_START

namespace algor{

    /// @brief Takes an input string {list} and it split 
    ///         it into multiplt string based on the 
    ///         delim character
    /// @return Empty vector if the string was not stripped
    std::vector<std::string> split_string_to_list(const std::string& list, const char delim = ' ') noexcept;
    
    /// @brief Takes an Input string {str} and removes 
    ///         all the character {to_be_removed} 
    ///         refernce into the string
    /// @return Fails :: 1
    Result clean(std::string& str, const char to_be_removed) noexcept;
}


__NAMESPACE_END

#endif

#if defined(BIL__ALGOR_IMPL) && !defined(ALGOR_IMPLEMENTATION)
#define ALGOR_IMPLEMENTATION

__NAMESPACE_START

Result algor::clean(std::string& str, const char to_be_removed) noexcept{

    for(size_t i = 0; i < str.length();){
        
        char& __c = str[i];
        if(__c == to_be_removed){
            std::memmove(str.data(), &str[i], (str.length() - i - 1));
            str.resize(str.length() - 1);
            continue;
        }
        i++;
    }

    Ok()
}

std::vector<std::string> algor::split_string_to_list(const std::string& list, const char delim) noexcept{
    std::vector<std::string> out;

    size_t s = 0;
    for(size_t i = 0; i <= list.length(); ++i){
        
        if((list[i] == delim) || (i >= list.length())){
            if(i - s > 0)
                out.push_back({&list[s], i - s});
            
            if(list[i + 1] == delim){
                while(list[++i] == delim && 
                        i <= list.length()) {}
            }
            s = i; ++s;
        }
    }
    
    return out;
}

__NAMESPACE_END


#endif
