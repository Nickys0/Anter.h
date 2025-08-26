# Anter.h
A simple only header library for parsing command line arguments

## EXAMPLE

```c
#define ANTER_IMPLEMENTATION
#include "Anter.h"

int main(int argc, char** argv) {
    bool flag = false;
    ant_init(argc, argv); /* for internally setting argc, argv */

    /* The last argument is not implemented yet so you can just put NULL in here */
    ant_flag_bool(&flag, "--bool", "-b", NULL); 

    /* ant_strerror returns a simple error msg, it uses an 
        internal buffer so no malloc required */
    if(ant_parse() != ANTERR_NONE)
        fprintf(stderr, "error: %s", ant_strerror());

    ...
    ant_clean( ); // Deallocates and setting the internal state to an invalid one
    return 0;
}
```

## Testing

For testing the library we use the [nob.h](https://github.com/tsoding/nob.h) library.
Here is how to test:

```bash
gcc -o nob nob.c
```

and then you can just run it to test all the files:

```bash
./nob
```

or you can just test a single file:

```bash
./nob test <FILE_BASENAME>
```

