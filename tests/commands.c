#define ANTER_IMPLEMENTATION
#include "Anter.h"

enum COMMANDS {
    HELP_COM, 
    NEW_COM,
    COMMAND_COUNT
};

int main(int argc, char** argv) {
    ant_init(argc, argv);

    // here we say to the library the existance of these commands
    ant_set_coms({
        [HELP_COM] = { "help", "basic_description", false }, 
        [NEW_COM] = { "new", "basic_description",  true  }
    });

    AnterCommand com = {0};
    if(ant_get_command(&com) != ANTERR_NONE){
        fprintf(stderr, "error: %s", ant_strerror());
        return 1;
    }

    fprintf(stdout, "%s command parsed, value: %s, type: %ld \n", com.str, com.val, com.idx);

    return 0;
}
