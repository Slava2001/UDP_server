#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "server.h"
#include "util.h"
#include "tlv_parser.h"
#include "commands_proc.h"

#define BUFF_SIZE 256

int debug_level = 0;

enum FDS {
    SERVER_FD,
    FDS_COUNT
};

int main(int argc, char *argv[]) {
    while(1) {
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

    struct pollfd fds[FDS_COUNT] = {};
    
    server_t server;
    if (init_server(&server, &fds[SERVER_FD])) {
        log_error("Failed to init server\n");
        return -1;
    };

    uint8_t buff[BUFF_SIZE];
    int recv_size;

    while(1) {

        if (poll(fds, FDS_COUNT, -1) < 0) {
            log_perror("Failed in poll.");
            return -1;
        }
        
        if (fds[SERVER_FD].revents) {
            recv_size = recv_dgram(&server, buff, BUFF_SIZE);
            if (recv_size < 0) {
                log_info("Failed to recv msg");
                continue;
            } 
       
            uint8_t *end_ptr = NULL;
            do {
                command_t c = tlv_parse_get(buff, recv_size, &end_ptr);
                if (command_proc(&c)) {
                    log_error("Failed to proccesing command");
                }
            } while(end_ptr);
        }
    }
    return 0;
}