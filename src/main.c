#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
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

    server_t server;

    if (init_server(&server)) {
        log_error("Failed to init server\n");
        return -1;
    };

    uint8_t buff[256];

    while(1) {
        int rc = recv_dgram(&server, buff, 256);
        if (rc < 0) {
            log_info("Failed to recv msg");
            continue;
        } 
        log_info("msg: %*s", rc, buff);
    }

    return 0;
}