#define ANTER_IMPLEMENTATION
#define ANTER_STRICT_REQUIRED_COMMAND
// #define ANTER_ALLOW_FLAGS_AS_COMMANDS
#include "Anter.h"

enum COMMANDS {
    HELP_COM, 
    NEW_COM,
    COMMAND_COUNT
};

int main(int argc, char** argv) {
    ant_init(argc, argv);

    // ant_set_coms({
    //     [HELP_COM] = { "help", "basic_description", false }, 
    //     [NEW_COM] = { "new", "basic_description",  true  }
    // });
        
    // here we say to the library the existance of these commands
    ant_set_command("help", "basic description", false);
    ant_set_command("new", "basic description",  true);

    AnterCommand com = {0};
    if(ant_parse(&com) != ANTERR_NONE){
        fprintf(stderr, "error: %s\n", ant_strerror());
        return 1;
    }

    fprintf(stdout, "%s command parsed, value: %s, type: %ld \n", com.str, com.val, com.idx);

    return 0;
}
