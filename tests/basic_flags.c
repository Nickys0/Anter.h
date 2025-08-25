#define ANTER_IMPLEMENTATION
#include "Anter.h"

int main(int argc, char** argv){

    bool boolean =     false;
    int i32 =          -1;
    char buffer[250] = {0};
    AnterDynamicString ds = {0};

    ant_init(argc, argv);

    ant_flag_bool(&boolean,     "--bool",    "-b", NULL);
    ant_flag_int(&i32,          "--i32",     "-n", NULL);
    ant_flag_fixed_buff(buffer, "--static",  "-s", NULL);
    ant_flag_dynamic_string(&ds,"--dynamic", "-d", NULL);

    AnterErrorKind kind;
    if((kind = ant_parse()) != ANTERR_NONE)
        printf("error: %s\n", ant_strerror());

    if(boolean)  fprintf(stdout, "--bool: TRUE\n");
    if(i32 >= 0) fprintf(stdout, "--i32: %d\n", i32);
    if(buffer[0] != '\0') fprintf(stdout, "--static: %s\n", buffer);
    if(ds.ptr != NULL) fprintf(stdout, "--dynamic: %s\n", ds.ptr);

    return (int)kind;
}
