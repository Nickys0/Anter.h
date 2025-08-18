#ifndef NOB_H_
#define NOB_H_

#ifndef NOB_ASSERT
#include <assert.h>
#define NOB_ASSERT assert
#endif /* NOB_ASSERT */

#ifndef NOB_REALLOC
#include <stdlib.h>
#define NOB_REALLOC realloc
#endif /* NOB_REALLOC */

#ifndef NOB_FREE
#include <stdlib.h>
#define NOB_FREE free
#endif /* NOB_FREE */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN
#    define _WINUSER_
#    define _WINGDI_
#    define _IMM_
#    define _WINCON_
#    include <windows.h>
#    include <direct.h>
#    include <shellapi.h>
#else
#    include <sys/types.h>
#    include <sys/wait.h>
#    include <sys/stat.h>
#    include <unistd.h>
#    include <fcntl.h>
#endif

// typedef enum{
//     NOB_INFO,
//     NOB_WARNING,
//     NOB_ERROR,
//     NOB_NO_LOGS,
//     NOB_BUILDER_INFO
// }Nob_Log_Level;

// // Any messages with the level below nob_minimal_log_level are going to be suppressed.
// extern Nob_Log_Level nob_minimal_log_level;
// void nob_log(Nob_Log_Level level, const char *fmt, ...);

typedef struct{
    const char **items;
    size_t count;
    size_t capacity;
}Nob_File_Paths;

typedef enum{
    NOB_FILE_REGULAR = 0,
    NOB_FILE_DIRECTORY,
    NOB_FILE_SYMLINK,
    NOB_FILE_OTHER,
}Nob_File_Type;

bool nob_mkdir_if_not_exists(const char *path);
bool nob_mkdir_if_not_exists_rec(const char *path);
int nob_mkdir(const char* path);

// RESULT:
// 1 -> dir empty
// 0 -> dir not empty
// -1 -> error(logged)
int nob_directory_empty(const char *path);
bool nob_copy_file(const char *src_path, const char *dst_path);
bool nob_copy_directory_recursively(const char *src_path, const char *dst_path);
bool nob_read_entire_dir(const char *parent, Nob_File_Paths *children);
bool nob_write_entire_file(const char *path, const void *data, size_t size);
Nob_File_Type nob_get_file_type(const char *path);
bool nob_delete_file(const char *path);

typedef struct{
    char *items;
    size_t count;
    size_t capacity;
}Nob_String_Builder;

bool nob_read_entire_file(const char *path, Nob_String_Builder *sb);
// like 'nob_read_entire_file' but it opens the file only as read mode ('r')
bool nob_read_entire_txt_file(const char *path, Nob_String_Builder *sb);

Nob_Fd nob_fd_open_for_read(const char *path);
Nob_Fd nob_fd_open_for_write(const char *path);
void nob_fd_close(Nob_Fd fd);

typedef struct{
    Nob_Proc *items;
    size_t count;
    size_t capacity;
}Nob_Procs;

bool nob_procs_wait(Nob_Procs procs);
bool nob_procs_wait_and_reset(Nob_Procs *procs);
bool nob_proc_wait(Nob_Proc proc);
bool nob_proc_wait_exit_code(Nob_Proc proc, Nob_Exit_Code* code);

// A command - the main workhorse of Nob. Nob is all about building commands an running them
typedef struct{
    const char **items;
    size_t count;
    size_t capacity;
}Nob_Cmd;

typedef struct{
    Nob_Fd *fdin;
    Nob_Fd *fdout;
    Nob_Fd *fderr;
}Nob_Cmd_Redirect;

void nob_cmd_render(Nob_Cmd cmd, Nob_String_Builder *render);

// Run command asynchronously
#define nob_cmd_run_async(cmd) nob_cmd_run_async_redirect(cmd, (Nob_Cmd_Redirect) {0})
Nob_Proc nob_cmd_run_async_and_reset(Nob_Cmd *cmd);
Nob_Proc nob_cmd_run_async_redirect(Nob_Cmd cmd, Nob_Cmd_Redirect redirect);
Nob_Proc nob_cmd_run_async_redirect_and_reset(Nob_Cmd *cmd, Nob_Cmd_Redirect redirect);

bool nob_cmd_run_sync(Nob_Cmd cmd);
bool nob_cmd_run_sync_and_reset(Nob_Cmd *cmd);
bool nob_run_sync_and_reset_getexode(Nob_Cmd *cmd, Nob_Exit_Code* code);
bool nob_cmd_run_sync_redirect(Nob_Cmd cmd, Nob_Cmd_Redirect redirect);
bool nob_cmd_run_sync_redirect_and_reset(Nob_Cmd *cmd, Nob_Cmd_Redirect redirect);

char* nob_temp_file(void);
char *nob_temp_strdup(const char *cstr);
char *nob_temp_strndup(const char* data, size_t len);
void *nob_temp_alloc(size_t size);
char *nob_temp_sprintf(const char *format, ...);
void nob_temp_reset(void);
size_t nob_temp_save(void);
void nob_temp_rewind(size_t checkpoint);

const char *nob_path_name(const char *path);
bool nob_rename(const char *old_path, const char *new_path);
int nob_needs_rebuild(const char *output_path, const char **input_paths, size_t input_paths_count);
int nob_needs_rebuild1(const char *output_path, const char *input_path);
int nob_file_exists(const char *file_path);
const char *nob_get_current_dir_temp(void);
bool nob_set_current_dir(const char *path);

typedef struct{
    size_t count;
    size_t capacity;
    const char** items;
}Nob_String_Arr;

typedef struct{
    const char** Includes;  // -I Option
    const char** Warnings;  // -W options
    const char** Libs;      // -L and -l option
    const char** others;    // -fdiagnostic-colors=always, std=c11, -ggdb, -DNOB etc ...
}Nob_Builder_Args;

typedef struct{
    const char* compiler; /* compiler to use */
    Nob_Builder_Args* args;
    const char* folder; /* where to get the src files */
    const char* f_out;  /* depends on the function */
    const char* t_file; /* depends on the function */
    const char* src_ext; /* src file extension */
}Nob_Builder;

char* nob_trans_ext(const char* path, const char* new_ext);
char* nob_trans_ext_to_obj(const char* path);
bool nob_get_dir_files_by_ext(const char* __p, const char* _ext, Nob_File_Paths* out);
bool nob_comp_directory_by_time_to_obj(Nob_Builder builder);
bool nob_comp_dir_texe_sm(Nob_Builder builder);
bool nob_comp_c_directory_by_time_to_obj(Nob_Builder builder);
bool nob_comp_directory_to_obj(Nob_Builder builder);
bool nob_comp_c_directory_to_obj(Nob_Builder builder);
bool nob_link_obj_files(Nob_Builder builder);
bool nob_comp_c_file_to_obj(Nob_Builder builder);
bool nob_comp_c_file_to_exe(Nob_Builder builder);
Nob_String_Builder* nob_get_shcom_output_stdout(Nob_String_Arr com, Nob_String_Builder* out);
Nob_String_Builder* nob_get_shcom_output_stderr(Nob_String_Arr com, Nob_String_Builder* out);
bool nob_split_string(Nob_String_Builder b, Nob_String_Arr* out, char delim);

void nob__go_rebuild_urself(int argc, char **argv, const char *source_path, ...);

typedef struct{
    size_t count;
    const char *data;
}Nob_String_View;

const char *nob_temp_sv_to_cstr(Nob_String_View sv);

Nob_String_View nob_sv_chop_by_delim(Nob_String_View *sv, char delim);
Nob_String_View nob_sv_chop_left(Nob_String_View *sv, size_t n);
Nob_String_View nob_sv_trim(Nob_String_View sv);
Nob_String_View nob_sv_trim_left(Nob_String_View sv);
Nob_String_View nob_sv_trim_right(Nob_String_View sv);
bool nob_sv_eq(Nob_String_View a, Nob_String_View b);
bool nob_sv_end_with(Nob_String_View sv, const char *cstr);
bool nob_sv_starts_with(Nob_String_View sv, Nob_String_View expected_prefix);
Nob_String_View nob_sv_from_cstr(const char *cstr);
Nob_String_View nob_sv_from_parts(const char *data, size_t count);

// NOB_TEST
