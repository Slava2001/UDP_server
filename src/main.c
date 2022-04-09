#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

int debug_level = 0;

int main(int argc, char *argv[]) {
    for (;;) {
        int c = getopt(argc, argv, "d");
        if (c == -1) {
            break;
        }
        switch (c) {
        case 'd':
            debug_level++;
            break;
        default:
            log_error("Unhandled argument: %c\n", c);
            return 1;
        }
    }

    log_info("Hello, world!\n");
    debug_log(1, "debug lvl > 1\n");
    debug_log(2, "debug lvl > 2\n");
    debug_log(3, "debug lvl > 3\n");

    return 0;
}