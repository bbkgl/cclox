#include <iostream>
#include "cclox.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        Repl();
    } else if (argc == 2) {
        RunFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: cclox [path]\n");
        exit(64);
    }
    return 0;
}
