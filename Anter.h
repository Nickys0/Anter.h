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
 *       ant_clean( ); // will deallocate all the memory and reseting the internal state as invalid
 *       return 0;
 *   }
 *   ```
 * 
 *   OPTIONS:
 *   -    #define ANTER_IMPLEMENTATION            It is used to the the library to include the implementation of 
 *                                                its self
 * 
 *   -    #define DDASH_EOF_CMD_LINE_FLAGS        If defines it meas that when the parser encounters the double 
 *                                                dashed it will stop parsing flags: "--\0"
 *
 *   -    #define ANTER_STRICT_REQUIRED_COMMAND   It means that argv[1] should always be a command, if argv[1] 
 *                                                is a flag, from the given one it cause errors
 * 
 *   -    #define ANTER_ALLOC_MULTIPLE_OF         It is used to calculate the number of bytes to allocate for 
 *                                                AnterDynamicString [DEFAULT: 32] 
 *
 *   -    #define ANTER_NO_STRICT_PARAMS          If defined anter will not thrown an error if an unknown args was
 *                                                passed.
 * 
 *   -    #define ANTER_ALLOW_FLAGS_AS_COMMANDS   If defined it will let use flags syntax as commands
 *   
 *   CONFIGURABLE_MACROS:
 *   
 *   -  ANTER_FREE & ANTER_REALLOC & ANTER_ALLOC  These are default from the stdlib(free, realloc, malloc) but 
 *                                                you can customize it, by defined all three of them
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
#include <errno.h>

#ifndef ANTER_ALLOC_MULTIPLE_OF
#   define ANTER_ALLOC_MULTIPLE_OF 32
#endif

#if defined(ANTER_REALLOC) && !defined(ANTER_FREE) && !defined(ANTER_ALLOC)\
    || !defined(ANTER_REALLOC) && defined(ANTER_FREE) && !defined(ANTER_ALLOC)\
    || !defined(ANTER_REALLOC) && !defined(ANTER_FREE) && defined(ANTER_ALLOC)
#   error "You must define ANTER_REALLOC, ANTER_FREE, ANTER_ALLOC, or neither."
#endif

#if !defined(ANTER_REALLOC) && !defined(ANTER_FREE)
#   include <stdlib.h>
#   define ANTER_REALLOC(p,s)       realloc(p,s)
#   define ANTER_FREE(p)            free(p)
#   define ANTER_ALLOC(s)           malloc(s)
#endif

// For stb_ds compatibility
#define STBDS_REALLOC(c, p, s)   ANTER_REALLOC(p,s)
#define STBDS_FREE(c,p)          ANTER_FREE(p)

/* This is a modified version of stb_ds.h library 
    you can found the original one at: https://github.com/nothings/stb
    You can found the license of stb_ds.h at the bottom of this file */

/* stb_ds.h - v0.67 - public domain data structures - Sean Barrett 2019

   This is a single-header-file library that provides easy-to-use
   dynamic arrays and hash tables for C (also works in C++).

   For a gentle introduction:
      http://nothings.org/stb_ds

   To use this library, do this in *one* C or C++ file:
      #define STB_DS_IMPLEMENTATION
      #include "stb_ds.h"

LICENSE
  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.
*/
// TODO: remove this
#ifndef STBDS_NO_SHORT_NAMES
#define arrlen      stbds_arrlen
#define arrlenu     stbds_arrlenu
#define arrput      stbds_arrput
#define arrpush     stbds_arrput
#define arrpop      stbds_arrpop
#define arrfree     stbds_arrfree
#define arraddn     stbds_arraddn // deprecated, use one of the following instead:
#define arraddnptr  stbds_arraddnptr
#define arraddnindex stbds_arraddnindex
#define arrsetlen   stbds_arrsetlen
#define arrlast     stbds_arrlast
#define arrins      stbds_arrins
#define arrinsn     stbds_arrinsn
#define arrdel      stbds_arrdel
#define arrdeln     stbds_arrdeln
#define arrdelswap  stbds_arrdelswap
#define arrcap      stbds_arrcap
#define arrsetcap   stbds_arrsetcap
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void * stbds_arrgrowf(void *a, size_t elemsize, size_t addlen, size_t min_cap);
extern void   stbds_arrfreef(void *a);

#ifdef __cplusplus
}
#endif

#define stbds_header(t)  ((stbds_array_header *) (t) - 1)
#define stbds_temp(t)    stbds_header(t)->temp
#define stbds_temp_key(t) (*(char **) stbds_header(t)->hash_table)

#define stbds_arrsetcap(a,n)   (stbds_arrgrow(a,0,n))
#define stbds_arrsetlen(a,n)   ((stbds_arrcap(a) < (size_t) (n) ? stbds_arrsetcap((a),(size_t)(n)),0 : 0), (a) ? stbds_header(a)->length = (size_t) (n) : 0)
#define stbds_arrcap(a)        ((a) ? stbds_header(a)->capacity : 0)
#define stbds_arrlen(a)        ((a) ? (ptrdiff_t) stbds_header(a)->length : 0)
#define stbds_arrlenu(a)       ((a) ?             stbds_header(a)->length : 0)
#define stbds_arrput(a,v)      (stbds_arrmaybegrow(a,1), (a)[stbds_header(a)->length++] = (v))
#define stbds_arrpush          stbds_arrput  // synonym
#define stbds_arrpop(a)        (stbds_header(a)->length--, (a)[stbds_header(a)->length])
#define stbds_arraddn(a,n)     ((void)(stbds_arraddnindex(a, n)))    // deprecated, use one of the following instead:
#define stbds_arraddnptr(a,n)  (stbds_arrmaybegrow(a,n), (n) ? (stbds_header(a)->length += (n), &(a)[stbds_header(a)->length-(n)]) : (a))
#define stbds_arraddnindex(a,n)(stbds_arrmaybegrow(a,n), (n) ? (stbds_header(a)->length += (n), stbds_header(a)->length-(n)) : stbds_arrlen(a))
#define stbds_arraddnoff       stbds_arraddnindex
#define stbds_arrlast(a)       ((a)[stbds_header(a)->length-1])
#define stbds_arrfree(a)       ((void) ((a) ? STBDS_FREE(NULL,stbds_header(a)) : (void)0), (a)=NULL)
#define stbds_arrdel(a,i)      stbds_arrdeln(a,i,1)
#define stbds_arrdeln(a,i,n)   (memmove(&(a)[i], &(a)[(i)+(n)], sizeof *(a) * (stbds_header(a)->length-(n)-(i))), stbds_header(a)->length -= (n))
#define stbds_arrdelswap(a,i)  ((a)[i] = stbds_arrlast(a), stbds_header(a)->length -= 1)
#define stbds_arrinsn(a,i,n)   (stbds_arraddn((a),(n)), memmove(&(a)[(i)+(n)], &(a)[i], sizeof *(a) * (stbds_header(a)->length-(n)-(i))))
#define stbds_arrins(a,i,v)    (stbds_arrinsn((a),(i),1), (a)[i]=(v))

#define stbds_arrmaybegrow(a,n)  ((!(a) || stbds_header(a)->length + (n) > stbds_header(a)->capacity) \
                                  ? (stbds_arrgrow(a,n,0),0) : 0)

#define stbds_arrgrow(a,b,c)   ((a) = stbds_arrgrowf_wrapper((a), sizeof *(a), (b), (c)))

typedef struct{
    size_t      length;
    size_t      capacity;
    void      * hash_table;
    ptrdiff_t   temp;
} stbds_array_header;

#ifdef __cplusplus
// in C we use implicit assignment from these void*-returning functions to T*.
// in C++ these templates make the same code work
template<class T> static T * stbds_arrgrowf_wrapper(T *a, size_t elemsize, size_t addlen, size_t min_cap) {
  return (T*)stbds_arrgrowf((void *)a, elemsize, addlen, min_cap);
}
#else
#define stbds_arrgrowf_wrapper            stbds_arrgrowf
#endif
/****************** END OF STB_DS HEADER ******************/

typedef const char* sStr; /* static string, Literal string, C String */
typedef const char* lStr; /* static string, Literal string, C String */

// This structure is used for flag string with a maximum capacity.
// If you wanna set a max flag val len you can use this one.
struct AnterBuffer {
    char*  ptr;     /* pointer to the string buffer  */
    size_t len;     /* length of the buffer in the AnterFixedString this is not used */
    size_t size;    /* max size of the string buffer */
};

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

        AnterDynamicString* dyms;
        AnterFixedString    fixs;
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
    ANTERR_FLAG_EXPECTED_VALUE = 2, 

    /* This is an error that is caused when the value of a flag cannot 
        be converted to the desired type.
    to convert to the desired value we use the std function strto* so if something
        went wrong you can even use errno to get more info */
    ANTERR_CONVERSION_FAILED = 3,

    /* This is an error that is caused when the FixedString isn't enough 
        for the given buffer */
    ANTERR_NOT_ENOUGH_MEMORY = 4,

    /* This is error is set when it wasn't provied any commands through 
        the command line.
        You can change this behaviour throughout some options: see at 
        the top for more details
        error_info.str = NULL, error_info.idx = 1 */
    ANTERR_EXPECTED_COMMAND = 5,

    /* It indicates that the passed command from argv[1] is unknown */
    ANTERR_UNKNOWN_COMMAND = 6,

    /* like ANTERR_FLAG_EXPECTED_VALUE but for the command */
    ANTERR_COMM_EXPECTED_VALUE = 7,

    /* When the parsed argument doesn't produced any result this is thrown */
    ANTERR_UNKNOWN_ARG = 8,

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
ANTER_API void ant_flag_dynamic_string(AnterDynamicString* saver, lStr long_flag_name, lStr short_flag_name, lStr description);
#define ant_flag_fixed_buff(saver, long_flag, short_flag, desc)\
        ant_flag_string((AnterFixedString){ .ptr = saver, .size = (sizeof(saver) / sizeof(saver[0])) },\
                                long_flag, short_flag, desc )

ANTER_API void ant_set_command(const char* str_com, const char* desc, bool expect_val);
/// @brief This function will save the command to later be parsed.
///         A command should be rigth after the bin_path as follow:
///         bin_path <com> [VALUE] [OPTIONS]
/// @warning For the moment we don't implicit support subcommands
ANTER_API void _ant_set_coms(const AnterCommandParams* coms, size_t coms_count);

#define ant_set_coms(...) _ant_set_coms(((AnterCommandParams[])__VA_ARGS__),\
                    (sizeof((AnterCommandParams[])__VA_ARGS__) / sizeof(AnterCommand)))

/// @brief This function will retrieve the  command from the argv.
ANTER_API AnterErrorKind ant_get_command(AnterCommand* out);

// TODO: we should add a function that only parses a command or a flag

/// @brief This function is a merge of ant_get_command and ant_get_flags so it actually parses the arguments 
///         to get both commads and flags, you can actually use this to parse only the flags is out is NULL
/// @param out Is where we save the command info if given from both the user and the programmer.
///             If is NULL it means you don't expect any command, so if something was passed that is not a
///             flag relative an error is returned, TODO: to avoid this behaviour you can define: ANTER_NO_STRICT_PARAMS
/// @return FAILS :: != ANTERR_NONE
/// The information about the error can be found in the ant_last_error var,
/// the position of the params of the single error can be found in the AnterErrorKind enum declaration 
ANTER_API AnterErrorKind ant_parse(AnterCommand* out);

/******************** ANTER ERROR HANDLERS ********************/

// It returns the structure info of the last error
ANTER_API AnterError ant_last_error_struct(void);

// Returns th elast error kind
ANTER_API AnterErrorKind ant_last_error(void);

// It copies a string rappresentaion of the last error into buf
ANTER_API char* ant_string_error(char* buf, size_t size);

// like ant_string_error but it used an internal buffer to create the string
ANTER_API lStr ant_strerror(void);

// It will free all the allocated memory
ANTER_API void ant_clean(void);

#endif // !_ANTER_H

//////////////////////////////////////////////////////////////////////////////
//
//   IMPLEMENTATION
//
#define ANTER_IMPLEMENTATION
#ifdef ANTER_IMPLEMENTATION

AnterFlag* g__AntFlags = NULL;              /* List of flags, to append flags we use the stb_ds library */
AnterCommandParams* g__AntComs = NULL;      /* List of commands */
AnterError g__AntLastError = { ANTERR_NONE };         /* like errno but more fancy */

int _argc = 0;
char** _argv = NULL;

/*************************** STB_DS IMPLEMENTATION ****************************/
#ifndef STBDS_ASSERT
#define STBDS_ASSERT_WAS_UNDEFINED
#define STBDS_ASSERT(x)   ((void) 0)
#endif

#ifdef STBDS_STATISTICS
#define STBDS_STATS(x)   x
size_t stbds_array_grow;
size_t stbds_hash_grow;
size_t stbds_hash_shrink;
size_t stbds_hash_rebuild;
size_t stbds_hash_probes;
size_t stbds_hash_alloc;
size_t stbds_rehash_probes;
size_t stbds_rehash_items;
#else
#define STBDS_STATS(x)
#endif
    
void *stbds_arrgrowf(void *a, size_t elemsize, size_t addlen, size_t min_cap){
  stbds_array_header temp={0}; // force debugging
  void *b;
  size_t min_len = stbds_arrlen(a) + addlen;
  (void) sizeof(temp);

  // compute the minimum capacity needed
  if (min_len > min_cap)
    min_cap = min_len;

  if (min_cap <= stbds_arrcap(a))
    return a;

  // increase needed capacity to guarantee O(1) amortized
  if (min_cap < 2 * stbds_arrcap(a))
    min_cap = 2 * stbds_arrcap(a);
  else if (min_cap < 4)
    min_cap = 4;

  //if (num_prev < 65536) if (a) prev_allocs[num_prev++] = (int *) ((char *) a+1);
  //if (num_prev == 2201)
  //  num_prev = num_prev;
  b = STBDS_REALLOC(NULL, (a) ? stbds_header(a) : 0, elemsize * min_cap + sizeof(stbds_array_header));
  //if (num_prev < 65536) prev_allocs[num_prev++] = (int *) (char *) b;
  b = (char *) b + sizeof(stbds_array_header);
  if (a == NULL) {
    stbds_header(b)->length = 0;
    stbds_header(b)->hash_table = 0;
    stbds_header(b)->temp = 0;
  } else {
    STBDS_STATS(++stbds_array_grow);
  }
  stbds_header(b)->capacity = min_cap;

  return b;
}

void stbds_arrfreef(void *a)
{
  STBDS_FREE(NULL, stbds_header(a));
}

/************************ END OF STB_DS IMPLEMENTATION ************************/

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
        flag.value_as.dyms = (AnterDynamicString*)saver;
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

AnterErrorKind __get_command_info(AnterCommand* out){
    assert(out != NULL && "INTERNAL FUNCTION: out should not be NULL!");

    out->idx = -1;
    int arr_len = (int) stbds_arrlen(g__AntComs);

    // we find the command index
    for(int i = 0; i < arr_len; ++i){
        if(strcmp(_argv[1], g__AntComs[i].str) == 0){
            out->idx = i; break;
        }
    }

    // The command was not found
    if(out->idx == -1)
        return __newError(_argv[1], 1, ANTERR_UNKNOWN_COMMAND);
  
    bool expected_value = g__AntComs[out->idx].expect_val;
    out->str = _argv[1];
    
    // If the command expects a value we need to get it
    if(expected_value){
        if(_argc <= 2)
            return __newError(NULL, 2, ANTERR_COMM_EXPECTED_VALUE);
        
        out->val = _argv[2];
    }else out->val = NULL; /* we ensure that the value is set to NULL */
    
    return ANTERR_NONE;
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

void ant_flag_dynamic_string(AnterDynamicString* saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(saver, long_flag_name, short_flag_name, description, ANTER_FLAG_DYNAMIC_STRING);
}

void ant_flag_string(AnterFixedString saver, lStr long_flag_name, lStr short_flag_name, lStr description){
    __add_flag(&saver, long_flag_name, short_flag_name, description, ANTER_FLAG_FIXED_STRING);
}

void _ant_set_coms(const AnterCommandParams* coms, size_t coms_count){
    assert(coms != NULL);
    for(size_t i = 0; i < coms_count; ++i)
        ant_set_command(coms[i].str, coms[i].des, coms[i].expect_val);
}

void ant_set_command(const char* str_com, const char* desc, bool expect_val){
    AnterCommandParams __c = {
        .str = str_com,
        .des = desc,
        .expect_val = expect_val
    };
    stbds_arrpush(g__AntComs, __c);
}

AnterErrorKind ant_parse(AnterCommand* out){
    assert((_argc != 0 || _argv != NULL) && "Call ant_init before this");

    int argc =      _argc;
    char** argv =   _argv;
    size_t _idx =   argc; // last index;

    __shift_args(&argc, &argv); /* we skip the bin path */

    lStr arg = argv[0];

#if defined(ANTER_STRICT_REQUIRED_COMMAND)
    if(arg == NULL) return __newError(NULL, 1, ANTERR_EXPECTED_COMMAND);
#else
    if(arg == NULL) return ANTERR_NONE;
#endif

    if(out != NULL) /* We expect a command */ {
        bool flag_syntax = (arg[0] == '-' && arg[1] != '\0')? true : false;
        int fidx = __flag_idx(arg);
        if(fidx >= 0) /* its a flag */
#if defined(ANTER_STRICT_REQUIRED_COMMAND) && !defined(ANTER_ALLOW_FLAGS_AS_COMMANDS)
        return __newError(arg, 1, ANTERR_EXPECTED_COMMAND);
#else
        { /* do nothing */ }
        #endif
        else {
#if !defined(ANTER_ALLOW_FLAGS_AS_COMMANDS)
            if(flag_syntax) return __newError(arg, 1, ANTERR_EXPECTED_COMMAND);
#endif
            AnterErrorKind kind = __get_command_info(out);
            if(kind != ANTERR_NONE) return kind;
            assert(__shift_args(&argc, &argv) != NULL);
            if(g__AntComs[out->idx].expect_val){
                assert(__shift_args(&argc, &argv) != NULL);
            }
        }
    }

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
                if(str_value == NULL) return __newError(NULL, _idx, ANTERR_FLAG_EXPECTED_VALUE);   
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
                AnterDynamicString* dyms = __f->value_as.dyms;
                dyms->len =  val_len;
                
                /* we always allocate multiples of ANTER_ALLOC_MULTIPLE_OF */
                dyms->size = (ANTER_ALLOC_MULTIPLE_OF * (val_len / ANTER_ALLOC_MULTIPLE_OF)) 
                                            + (ANTER_ALLOC_MULTIPLE_OF * ((val_len % ANTER_ALLOC_MULTIPLE_OF) > 0)); 
                dyms->ptr = (char*) ANTER_ALLOC(dyms->size);

                assert(dyms->ptr != NULL);
                strcpy(dyms->ptr, str_value); /* if it fails it will crash the program */
            }break;

            case ANTER_FLAG_FIXED_STRING: {
                size_t val_len = strlen(str_value);
                // __f->value_as.fixs.len = val_len; // to do this you need to pass the fixed buffer as a pointer
                if(val_len > __f->value_as.fixs.size)
                    return __newError(str_value, (_idx - argc), ANTERR_NOT_ENOUGH_MEMORY);
                
                strcpy(__f->value_as.fixs.ptr, str_value); /* if it fails it will crash the program */
            }break;

            default:
                assert(0 && "Something went wrong: This flag type wasn't expected");
            }
        } 
#if !defined(ANTER_NO_STRICT_PARAMS)
        else{
            return __newError(arg, (_idx - argc) - 1, ANTERR_UNKNOWN_ARG);
        }
#endif
    }
    
    return ANTERR_NONE;
}

/* This will get the command without the flags, If you wanne get both you should use ant_parse! */
AnterErrorKind ant_get_command(AnterCommand *out){
    assert((_argc != 0 || _argv != NULL) && "Call ant_init before this");
    assert(out != NULL && "Plese provide the structure where to save the command info!");

    if(_argc <= 1) {
#if defined(ANTER_STRICT_REQUIRED_COMMAND)
        return __newError(NULL, 1, ANTERR_EXPECTED_COMMAND);
#else
        return ANTERR_NONE;
#endif
    }

    return __get_command_info(out);
}

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
        snprintf(buf, size, "Unknown flag: '%s'", g__AntLastError.arg);
    } break;
    
    case ANTERR_FLAG_EXPECTED_VALUE: {
        snprintf(buf, size, "The flag '%s' expected a value but found: <NULL>", _argv[g__AntLastError.arg_idx - 1]);
    } break;

    case ANTERR_CONVERSION_FAILED: {
        snprintf(buf, size, "The flag's value convertion failed: '%s'", _argv[g__AntLastError.arg_idx - 2]);
    } break;

    case ANTERR_NOT_ENOUGH_MEMORY: {
        snprintf(buf, size, "Not enough memory!");
    } break;

    case ANTERR_EXPECTED_COMMAND:{
        snprintf(buf, size, "Expected a command but found: '%s'", g__AntLastError.arg);
    }break;

    case ANTERR_UNKNOWN_COMMAND:{
        snprintf(buf, size, "Unknown command: %s", g__AntLastError.arg);
    }break;

    case ANTERR_COMM_EXPECTED_VALUE:{
        snprintf(buf, size, "The command: '%s' expected a value but found: <NULL>", _argv[g__AntLastError.arg_idx - 1]);
    }break;

    case ANTERR_UNKNOWN_ARG:{
        snprintf(buf, size, "Unknown argument given at argv[<%ld>]: %s", g__AntLastError.arg_idx, g__AntLastError.arg);
    }break;

    default:{ }
    }

    static_assert(((ANTERR_COUNT - ANTERR_UNKNOWN_ARG) == 1), "PLEASE UPDATE THIS FUNCTION");

    return buf;
}

lStr ant_strerror(void){
    static char buffer[512];
    return ant_string_error(buffer, sizeof(buffer));
}

void ant_clean(void){
    stbds_arrfree(g__AntComs);
    stbds_arrfree(g__AntFlags);
    
    g__AntLastError.arg = NULL;
    g__AntLastError.arg_idx = 0;
    g__AntLastError.kind = ANTERR_NONE;

    _argc = 0;
    _argv = NULL;
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
 *      - MINOR is incremented when new functions and/or types are added.
 *      - MAJOR update should be just a periodic cleanup of the deprecated functions and types
 *        without really modifying any existing functionality.
 * 
 *      
 *      VERSION HISTORY
 * 
 *      |    DATE    |  VERSION  |              DESCRIPTION
 *      |            |           |
 *      | (26/08/25) |   0.3.0   | ++ __get_command_info [INTERNAL]
 *      |            |           | -+ modified ant_parse() that nows parses commands and flags altogether
 *      |            |           | ++ ant_clean
 *      |            |           | ++ ant_set_command
 *      |            |           | ++ NEW OPTION: ANTER_ALLOW_FLAGS_AS_COMMANDS
 *      |            |           | ++ NEW OPTION: ANTER_NO_STRICT_PARAMS
 *      |            |           |
 *      | (26/08/25) |   0.2.4   | ++ Fixing ant_get_command function, we ensure that by default idx is equal to -1
 *      |            |           | -+ Mofified the ant_parse function for AnterDynamicString flag so that we always
 *      |            |           |    allocate a multiple of ANTER_ALLOC_MULTIPLE_OF
 *      |            |           | ++ Adding:
 *      |            |           |    - ANTER_ALLOC         [MACRO]
 *      |            |           |    - ANTER_REALLOC       [MACRO]
 *      |            |           |    - ANTER_FREE          [MACRO]
 *      |            |           | 
 *      | (25/08/25) |   0.2.3   | Integrating stb_ds.h library so that this can become an header only library
 *      |            |           |
 *      | (25/08/25) |   0.2.2   | Fixing AnterDynamicString problems
 *      |            |           |
 *      | (24/08/25) |   0.2.1   | Some bug fixes
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
 **/

/*
    TODOS:
    - We should allow these flag value regexp: [!!!]
        flag=value | flag value

    - Add an implicit help command or flag based on a macro definition [!]
*/

/*
----------------------------*- STB_DS.H LICENSE -*---------------------------------
Copyright (c) 2019 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
----------------------------*- STB_DS.H LICENSE -*---------------------------------
*/
