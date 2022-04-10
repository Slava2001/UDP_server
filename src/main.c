#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
#include "util.h"
#include "tlv_parser.h"

#define BUFF_SIZE 256

int debug_level = 0;

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

    server_t server;

    if (init_server(&server)) {
        log_error("Failed to init server\n");
        return -1;
    };

    uint8_t buff[BUFF_SIZE];

    while(1) {
        int recv_size = recv_dgram(&server, buff, BUFF_SIZE);
        if (recv_size < 0) {
            log_info("Failed to recv msg");
            continue;
        } 

        uint8_t *end_ptr = NULL;
        do {
            command_t c = tlv_parse_get(buff, recv_size, &end_ptr);
            log_raw("recv command: tag: 0x%04x len: 0x%04x data: ", c.tag, c.length);
            
            if (c.tag == 0xbeef) {
                log_raw("%s\n", c.data);
            } else {
                for (int i = 0; i < c.length; ++i) {
                    log_raw("%02X", c.data[i]);
                }
                log_raw("\n");
            }
        } while(end_ptr);
    }
    return 0;
}