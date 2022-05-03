#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "server.h"
#include "util.h"
#include "tlv_parser.h"
#include "commands_proc.h"
#include "queue.h"

#define BUFF_SIZE 256

int debug_level = 0;

#define SERVER_FD 0
#define MAX_FDS_COUNT 10


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
            log_error("Unhandled argument: %c", c);
            return 1;
        }
    }

    struct pollfd fds[MAX_FDS_COUNT] = {};
    for (int i = 1; i < MAX_FDS_COUNT; ++i) {
        fds[i].fd = -1;
    }
    struct queue cmds[MAX_FDS_COUNT] = {};
    
    server_t server;
    if (init_server(&server, &fds[SERVER_FD])) {
        log_error("Failed to init server");
        return -1;
    };

    if (init_commands()) {
        log_error("Failed to init commands");
        return -1;
    }

    uint8_t buff[BUFF_SIZE];
    int recv_size;

    while(1) {

        int rc;
        if ((rc = poll(fds, MAX_FDS_COUNT, -1)) < 0) {
            log_perror("Failed in poll.");
            return -1;
        }
        log_debug(1, "poll rc: %d", rc);
        if (fds[SERVER_FD].revents) {
            recv_size = recv_dgram(&server, buff, BUFF_SIZE);
            if (recv_size < 0) {
                log_info("Failed to recv msg");
                continue;
            } 
       
            uint8_t *end_ptr = NULL;
            do {
                command_t c = tlv_parse_get(buff, recv_size, &end_ptr);
                struct pollfd fd;

                if (get_command_fd(&c, &fd)) {
                    log_error("Failed to get command fd");
                    continue;
                }

                int i = 1;
                do {
                    if (fds[i].fd == -1 || fds[i].fd == fd.fd) {
                        break;
                    }
                    ++i;
                } while(i < MAX_FDS_COUNT);
                if (i >= MAX_FDS_COUNT) {
                    log_error("FD queue is full");
                    return -1;
                }
                
                fds[i] = fd;

                log_debug(1, "i: %d", i);
                
                queue_put_element(&cmds[i], &c);
            } while(end_ptr);
        }

        for (int i = 1; i < MAX_FDS_COUNT; ++i) {
            if (fds[i].revents) {
                command_t c;
                if (queue_get_element(&cmds[i], &c)) {
                    fds[i].fd = -1;
                    continue;
                }
                if (command_proc(&c)) {
                    log_error("Failed to proccesing command");
                }
            }
        }              
    }
    return 0;
}