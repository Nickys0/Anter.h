/** 
 * 
 *   How to use:
 *   
 *   ```c
 *   #define [OPTIONS]
 *   #include "Anter.h" 
 *  
 *   int main(int argc, char** argv){
 *       bool name = false;
 *       int num = 0;
 *       char buffer[250];
 *       AnterDynamicString dynamic_string = {0};
 *   
 *       ant_flag_bool(&name, "--name", NULL, NULL);
 *       ant_flag_int(&num,   "--num",  "-n", NULL);
 *       ant_flag_string(&Anter_NewFxStr(buffer), "--surname", NULL, NULL);
 *       ant_flag_dynamic_string(&dynamic_string, "--dynamic", "-d", NULL);
 *   
 *       ant_parse(argc, argv);
 *       // every other related function 
 *       ...
 *   
 *       return 0;
 *   }
 *   ```
 * 
 *   OPTIONS:
 *   -    #define DDASH_EOF_CMD_LINE_FLAGS        If defines it meas that when the parser encounters the double 
 *                                                dashed it will stop parsing flags: "--\0"
 *
 *   -    #define ANTER_IMPLEMENTATION            It is used to the the library to include the implementation of 
 *                                                its self
 *
 *   -    #define ANTER_STRICT_REQ_COM            It means that argv[1] should always be a command, if argv[1] 
 *                                                is a flag, from the given one it cause errors
 *  
 *   -    #define ANTER_ALLOW_DASHES_FOR_COMMANDS It allows the use of dashes with the command
 *       
 */

#ifndef _ANTER_H
#define _ANTER_H

#ifdef ANTER_IMPLEMENTATION
#   define STB_DS_IMPLEMENTATION
#   define ANTER_API
#else
#   define ANTER_API extern
#endif


#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
// #include <errno.h>

#include "vendor/stb/stb_ds.h"

typedef const char* sStr; /* static string, Literal string, C String */
typedef const char* lStr; /* static string, Literal string, C String */

// This structure is used for flag string with a maximum capacity.
// If you wanna set a max flag val len you can use this one.
struct AnterBuffer {
    char*  ptr;     /* pointer to the string buffer */
    size_t size;    /* max size of the string buffer */
};

// typedef struct {
    //     char*  ptr;
    //     size_t len;
    // } AnterDynamicString;
// For flag string with an unknown size, the string buffer will 
// be allocate in the heap througth std malloc

typedef struct AnterBuffer AnterDynamicString;
typedef struct AnterBuffer AnterFixedString;

// Different Anter flag Types
typedef enum {
    ANTER_FLAG_BOOL = 0,    // Didn't expect any value
    
    // All of these expects a value
    ANTER_FLAG_INT,
    ANTER_FLAG_FLOAT,
    ANTER_FLAG_FIXED_STRING,
    ANTER_FLAG_DYNAMIC_STRING,
    
    ANTER_FLAG_TYPE_COUNT
} AnterFlagType;

typedef struct __ant_flag_struct {

    AnterFlagType type;     /* Type of the flag */

    sStr long_flag_name;    /* Long flag name, without the double dashes */
    sStr short_flag_name;   /* Short flag name, without the single dash */
    sStr description;       /* Description of the flag [NOT SUPPORTED YET] */
    
    union {
        int32_t*  i32;
        float*    f32;
        bool*     boolean;

        AnterDynamicString dyms;
        AnterFixedString   fixs;
    }value_as;

} AnterFlag;


// A command should be declared immidiately after the bin_path as follow:
// bin_path <com> [com_value] [OPTIONS/FLAGS]
// The command_value depends on the command its self
typedef struct __ant_com_struct {

    lStr str;   /* string rappresentation of the command */
    long idx;   /* index of the command from the given args */
    lStr val;   /* Pointer to the value, if required */

} AnterCommand;

typedef struct __ant_com_params_struct {

    lStr str;           /* string rappresentation of the command */
    lStr des;           /* command description */
    
    /* I don't like this, I mean for just one bit we will alloc 8bytes, 
        maybe we can hide it in the description somehow? */
    bool expect_val;

} AnterCommandParams;


typedef enum AnterErrorKind {

    ANTERR_NONE = 0,

    /* whene parsing the flag wasn't found in the saved flag that 
        where provided from the function flag_* */
    ANTERR_UNKNOWN_FLAG = 1,
    
    /* when a flag or a command that expected a value but NULL was found, 
        this error is returned */
    ANTERR_EXPECTED_VALUE = 2, 

    /* This is an error that is caused when the value of a flag cannot 
        be converted to the desired type.
    to convert to the desired value we use the std function strto* so if something
        went wrong you can even use errno to get more info */
    ANTERR_CONVERSION_FAILED = 3,

    /* This is an error that is caused when the FixedString isn't enough 
        for the given buffer */
    ANTERR_NOT_ENOUGH_MEMORY = 4,

    /* This is error is set when the option 'ANTER_STRICT_REQ_COM' was defined and
        it wasn't provied any commands through the command line.
        error_info.str = NULL, error_info.idx = 1 */
    ANTERR_EXPECTED_COMMAND = 5,

    /* It indicates that the passed command from argv[1] is unknown */
    ANTERR_UNKNOWN_COMMAND = 6,

    ANTERR_COUNT

} AnterErrorKind;


typedef struct {

    AnterErrorKind kind;

    // The arg that caused the error [not always is set, it depends on the error]
    lStr arg; /* this is equal to argv[arg_idx] */ 

    size_t arg_idx; /* tha arg index whitin argv that caused the error */

}AnterError;

/****************** ANTER_API ******************/

// This should be the first function to be called
ANTER_API void ant_init(int argc, char** argv);

/// Imagine we have the flag '--time, -t' here we will call this function has
///          ant_flag_bool(&bool, "time", "t", "simple description");
/// If the flag was passed througth the command line bool will be set to true 
/// otherwise it will remain we its default value
///
/// @param saver Pointer to a variable where it will be save a value
/// @param long_flag_name Flag name, withou the double dashes (Can be NULL)
/// @param short_flag_name Flag name, withou the single dash (Can be NULL)
/// @param description String that describes the flag
/// @warning Only one of the two flag name can be null not both otherwise an assertion will fail

ANTER_API void ant_flag_bool(bool* saver, lStr long_flag_name, lStr short_flag_name, lStr description);
ANTER_API void ant_flag_int(int32_t* saver, lStr long_flag_name, lStr short_flag_name, lStr description);
ANTER_API void ant_flag_float(float* saver, lStr long_flag_name, lStr short_flag_name, lStr description);
ANTER_API void ant_flag_string(AnterFixedString saver, lStr long_flag_name, lStr short_flag_name, lStr description);
ANTER_API void ant_flag_dynamic_string(AnterDynamicString saver, lStr long_flag_name, lStr short_flag_name, lStr description);

#define ant_flag_fixed_buff(saver, long_flag, short_flag, desc)\
        ant_flag_string((AnterFixedString){ .ptr = saver, .size = (sizeof(saver) / sizeof(saver[0])) },\
                                long_flag, short_flag, desc )

/// @brief This function will save the command to later be parsed.
///         A command should be rigth after the bin_path as follow:
///         bin_path <com> [VALUE] [OPTIONS]
/// @warning For the moment we don't implicit support subcommands
ANTER_API void _ant_set_coms(const AnterCommandParams* coms, size_t coms_count);

#define ant_set_coms(...) _ant_set_coms(((AnterCommandParams[])__VA_ARGS__),\
                    (sizeof((AnterCommandParams[])__VA_ARGS__) / sizeof(AnterCommand)))

/// @brief This function will retrieve the  command from the argv.
///         If the command is a registered flag it will fail the function. 
///         To avoid this behavior you can set the 'ANTER_ALLOW_DASHES_FOR_COMMANDS' option
ANTER_API AnterErrorKind ant_get_command(AnterCommand* out);


/// @brief argc and argv should be passed as the same as the main function, no shifting required
/// @return FAILS :: != ANTERR_NONE
/// The information about the error can be found in the ant_last_error var,
/// the position of the params of the single error can be found in the AnterErrorKind enum declaration 
ANTER_API AnterErrorKind ant_parse(void);

/******************** ANTER ERROR HANDLERS ********************/

// It returns the structure info of the last error
ANTER_API AnterError ant_last_error_struct(void);

// Returns th elast error kind
ANTER_API AnterErrorKind ant_last_error(void);

// It copies a string rappresentaion of the last error into buf
ANTER_API char* ant_string_error(char* buf, size_t size);

// like ant_string_error but it used an internal buffer to create the string
ANTER_API lStr ant_strerror(void);

#endif // !_ANTER_H

#ifdef ANTER_IMPLEMENTATION

AnterFlag* g__AntFlags = NULL;              /* List of flags, to append flags we use the stb_ds library */
AnterCommandParams* g__AntComs = NULL;      /* List of commands */
AnterError g__AntLastError = { 0 };         /* like errno but more fancy */

int _argc = 0;
char** _argv = NULL;

/*************** ANTER INTERNALS ***************/
void __add_flag(void *saver, lStr long_flag_name, lStr short_flag_name, lStr description, AnterFlagType type){
    assert(saver != NULL && "Saver pointer cannot be NULL");
    assert((long_flag_name != NULL && short_flag_name != NULL) && "At least one flag name must be provided");

    /// @QST: Maybe we should duplicate the strings?
    AnterFlag flag;
    flag.type =             type;
    flag.long_flag_name =   long_flag_name;
    flag.short_flag_name =  short_flag_name;
    flag.description =      description;

    switch(type){

    case ANTER_FLAG_BOOL: {
        flag.value_as.boolean = (bool*)saver;
    }break;

    case ANTER_FLAG_INT: {
        flag.value_as.i32 = (int32_t*)saver;
    }break;

    case ANTER_FLAG_FLOAT: {
        flag.value_as.f32 = (float*)saver;
    }break;

    // Error: for dynamic string the user should know the size of the string!
    case ANTER_FLAG_DYNAMIC_STRING: {
        flag.value_as.dyms = *((AnterDynamicString*)saver);
    }break;
    
    case ANTER_FLAG_FIXED_STRING: {
        flag.value_as.fixs = *((AnterFixedString*)saver);
    }break;

    default:
        assert(0 && "Please update this function!");
    }
   
    stbds_arrpush(g__AntFlags, flag);
}

// We pop from left to rigth the args
lStr __shift_args(int *argc, char ***argv){
    if(*argc <= 0) return NULL;

    lStr result = **argv;
    (*argv)++; (*argc)--;

    return result;
}

// We search the flag througth the g__AntFlags
// FAILS :: -1
int __flag_idx(lStr name) {
    assert(name != NULL);
    int __len = stbds_arrlen(g__AntFlags);

    for(int j = 0; j < __len; ++j){
        
        /* single dashed flag */
        if(name[0] == '-' && name[1] != '-' && 
            strcmp(name, g__AntFlags[j].short_flag_name) == 0) return j;
        
        // double dashed flag
        else if(strcmp(name, g__AntFlags[j].long_flag_name) == 0) return j;
    }

    return -1;
}

AnterErrorKind __newError(lStr arg, size_t idx, AnterErrorKind kind){
    g__AntLastError.arg = arg;
    g__AntLastError.arg_idx = idx;
    return (g__AntLastError.kind = kind);
}

void ant_init(int argc, char **argv){
    _argc = argc;
    _argv = argv;
}

/************************** ANTER API IMPLEMENTATION **************************/
void ant_flag_bool(bool *saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(saver, long_flag_name, short_flag_name, description, ANTER_FLAG_BOOL);
}

void ant_flag_int(int32_t *saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(saver, long_flag_name, short_flag_name, description, ANTER_FLAG_INT);
}

void ant_flag_float(float *saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(saver, long_flag_name, short_flag_name, description, ANTER_FLAG_FLOAT);
}

void ant_flag_dynamic_string(AnterDynamicString saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(&saver, long_flag_name, short_flag_name, description, ANTER_FLAG_DYNAMIC_STRING);
}

void ant_flag_string(AnterFixedString saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(&saver, long_flag_name, short_flag_name, description, ANTER_FLAG_FIXED_STRING);
}

void _ant_set_coms(const AnterCommandParams* coms, size_t coms_count){
    assert(coms != NULL);

    for(size_t i = 0; i < coms_count; ++i){
        AnterCommandParams __c = {
            .str = coms[i].str,
            .des = coms[i].des,
            .expect_val = coms[i].expect_val
        };
        stbds_arrpush(g__AntComs, __c);
    }
}

AnterErrorKind ant_parse(void){
    assert(_argc != 0 || _argv != NULL);

    int argc =      _argc;
    char** argv =   _argv;
    size_t _idx =   argc; // last index;


    __shift_args(&argc, &argv); /* we skip the bin path */

    lStr arg = NULL;

    while((arg = __shift_args(&argc, &argv)) != NULL) {

#if defined(DDASH_EOF_CMD_LINE_FLAGS)
        // TODO:
        // we should save the position of this double dashed so that 
        // the user can later get the later args
        if(strcmp(arg, "--") == 0) break;
#endif

        if(arg[0] == '-' && arg[1] != '\0') /* Its a flag */ {
            
            // We search the flag within our saved flags
            int f_pos = __flag_idx(arg);
            if(f_pos == -1) /* not found */
                return __newError(arg, (_idx - argc) , ANTERR_UNKNOWN_FLAG);

            AnterFlag* __f = &g__AntFlags[f_pos];
            
            lStr str_value = NULL;
            if(__f->type != ANTER_FLAG_BOOL) {
                // If its not a boolean we need to get the value
                str_value = __shift_args(&argc, &argv);
                if(str_value == NULL) return __newError(NULL, _idx, ANTERR_EXPECTED_VALUE);   
            }

            // Here we convert the string_value to what the user asked 
            // for that specific flag
            switch(__f->type){

            case ANTER_FLAG_BOOL: {
                *__f->value_as.boolean = true;
            }break;

            case ANTER_FLAG_INT: {
                char* end = NULL;
                int32_t val = strtol(str_value, &end, 10);

                if(end == str_value || *end != '\0')
                    return __newError(str_value, (_idx - argc), ANTERR_CONVERSION_FAILED);
                
                *__f->value_as.i32 = val;
            }break;

            case ANTER_FLAG_FLOAT: {
                char* end = NULL;
                float val = strtof(str_value, &end);
                if(end == str_value || *end != '\0')
                    return __newError(str_value, (_idx - argc), ANTERR_CONVERSION_FAILED);
                
                *__f->value_as.f32 = val;
            }break;

            case ANTER_FLAG_DYNAMIC_STRING: {
                size_t val_len = strlen(str_value);

                __f->value_as.dyms.size = val_len;
                __f->value_as.dyms.ptr = (char*) malloc(__f->value_as.dyms.size);
                assert(__f->value_as.dyms.ptr != NULL);

                strcpy(__f->value_as.dyms.ptr, str_value); /* if it fails it will crash the program */
            }break;

            case ANTER_FLAG_FIXED_STRING: {
                size_t val_len = strlen(str_value);
                
                if(val_len > __f->value_as.fixs.size)
                    return __newError(str_value, (_idx - argc), ANTERR_NOT_ENOUGH_MEMORY);
                
                strcpy(__f->value_as.fixs.ptr, str_value); /* if it fails it will crash the program */
            }break;

            default:
                assert(0 && "Something went wrong!");
            }
        }
    }
    
    return ANTERR_NONE;
}


AnterErrorKind ant_get_command(AnterCommand *out){
    assert(out != NULL && "Plese provide the structure where to save the command info!");

    if(_argc <= 1) {
#if defined(ANTER_STRICT_REQ_COM)
        return __newError(NULL, 1, ANTERR_EXPECTED_COMMAND);
#else
        return ANTERR_NONE;
#endif
    }

    int arr_len = (int) stbds_arrlen(g__AntComs);

    // we find the command index
    int com_idx = -1;
    for(int i = 0; i < arr_len; ++i){
        if(strcmp(_argv[1], g__AntComs[i].str) == 0){
            com_idx = i; break;
        }
    }

    // The command was not found from the registered
    if(com_idx == -1) 
        return __newError(_argv[1], 1, ANTERR_UNKNOWN_COMMAND);

    bool expected_value = g__AntComs[com_idx].expect_val;

    out->idx = com_idx;
    out->str = _argv[1];
    
    // If the command expects a value we need to get it
    if(expected_value){
        if(_argc <= 2)
            return __newError(NULL, 2, ANTERR_EXPECTED_VALUE);
        
        out->val = _argv[2];
    }else out->val = NULL; /* we ensure that the value is set to NULL */
    
    return ANTERR_NONE;
}

/*
sStr ant_com_str(lStr com){
    assert(com != NULL);

    int arr_len = (int) stbds_arrlen(g__AntComs);

    // we check the command was actually provided through 'ant_com'
    int com_idx = -1;
    for(int i = 0; i < arr_len; ++i){
        if(strcmp(com, g__AntComs[i].str) == 0){
            com_idx = i; break;
        }
    }
    // If this happen it means the user didn't call before this the func ant_com to save the command
    assert(com_idx != -1);

    bool flag_expects_value = g__AntComs[com_idx].expect_value;

    if(_argc <= 1) {
#if defined(ANTER_STRICT_REQ_COM)
        __newError("<NULL>", 1, ANTERR_EXPECTED_COMMAND);
#endif
        return NULL;
    }

    // Now we see wich command was used
    if(strcmp(_argv[1], com) == 0) {

    }

    return NULL;
}
*/

/******************** ANTER ERROR HANDLERS ********************/

AnterError ant_last_error_struct(void){
    return g__AntLastError;
}

AnterErrorKind ant_last_error(void){
    return g__AntLastError.kind;
}

char* ant_string_error(char *buf, size_t size){
    
    switch (g__AntLastError.kind){
    
    case ANTERR_NONE: {
        strncpy(buf, "None", size);
    } break;

    case ANTERR_UNKNOWN_FLAG: {
        snprintf(buf, size, "Unknown flag: <%s>", g__AntLastError.arg);
    } break;
    
    case ANTERR_EXPECTED_VALUE: {
        snprintf(buf, size, "The flag '%s' expected a value but found: <NULL>", _argv[g__AntLastError.arg_idx - 1]);
    } break;

    case ANTERR_CONVERSION_FAILED: {
        // TODO: AntLastError.arg should point to the value of the flag not the flag its self!!
        snprintf(buf, size, "Conversation type for flag '%s' failed: %s ", g__AntLastError.arg, strerror(errno));
    } break;
    
    case ANTERR_NOT_ENOUGH_MEMORY: {
        snprintf(buf, size, "Not enough memory!");
    } break;
    
    case ANTERR_EXPECTED_COMMAND:{
        snprintf(buf, size, "Expected a command but found: <NULL>");
    }break;

    case ANTERR_UNKNOWN_COMMAND:{
        snprintf(buf, size, "Unknown command: %s", g__AntLastError.arg);
    }break;

    default:{ }
    }

    static_assert(((ANTERR_COUNT - ANTERR_UNKNOWN_COMMAND) == 1), "PLEASE UPDATE THIS FUNCTION");

    return buf;
}

lStr ant_strerror(void){
    static char buffer[512];
    return ant_string_error(buffer, sizeof(buffer));
}

#endif // ANTER_IMPLEMENTATION

/**
 * 
 *                      -----------------------------
 *                          -*- ANTER LIBRARY -*-
 *                      -----------------------------    
 *  
 *      Version Conventions:
 *      - Modifying comments does not update the version.
 *      - PATCH is incremented in case of a bug fix or refactoring without touching the API.
 *      - MINOR is incremented when new functions and/or types are added in a way that does
 *        not break any existing user code.
 *      - MAJOR update should be just a periodic cleanup of the deprecated functions and types
 *        without really modifying any existing functionality.
 * 
 *      
 *      VERSION HISTORY
 * 
 *      |    DATE    |  VERSION  |              DESCRIPTION
 *      |            |           |
 *      | (15/08/25) |   0.2.0   | ++ AnterCommand              [STRUCT]
 *      |            |           | ++ AnterCommandPARAMS        [STRUCT]
 *      |            |           | ++ ant_init                  [FUNC]
 *      |            |           | ++ ant_set_coms              [FUNC]
 *      |            |           | ++ ant_get_command           [FUNC]
 *      |            |           | ** Mofied the ant_parse func
 *      |            |           |
 *      | (10/08/25) |   0.1.0   | First release of the library
 *      |            |           |
 *          
 * 
 **/

/*
    TODOS:
    - Add the supports for the commands:
        <exe> <com> [options] ...
        <com>
        |
        | -> <com> [OPTIONS]
        |
        | -> <com> <subcom> [OPTIONS]   {STILL NOT IMPLEMENTED}
        | -> <com> <value> [OPTIONS]
    These should be the supported syntax for the commands

    - We should allow these flag value regexp:
        flag=value | flag value

    - Add an implicit help command or flag based on a macro definition
*/
