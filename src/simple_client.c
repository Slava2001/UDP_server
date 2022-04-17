#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>

#include "util.h"
#include "server.h"
#include "tlv_parser.h"

#define BUFF_SIZE 256

int debug_level = 0;

int main(int argc, char *argv[]) {
    char *msg = "Hello, world";
    int msg_len = strlen(msg)+1;

    while(1) {
        int c = getopt(argc, argv, "dm:");
        if (c == -1) {
            break;
        }
        switch (c) {
        case 'd':
            debug_level++;
            break;
        case 'm':
            msg = optarg;
            msg_len = strlen(msg)+1;
            break;
        default:
            log_error("Unhandled argument: %c\n", c);
            return 1;
        }
    }

    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd == -1) {
        log_perror("Failed to create socket.");
        return -1;
    }
    
    struct sockaddr_un server_addr = { };
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCKET_PATH, 108);


    uint8_t buff[BUFF_SIZE]; 
    
    command_t command;
    command.tag = 0xbeef;
    command.length = msg_len;
    command.data = (uint8_t*)msg;

    uint8_t *end_ptr = NULL;
    int packet_len = tlv_parse_put(&command, buff, BUFF_SIZE, &end_ptr);
    if (packet_len < 0) {
        log_error("Failed to put cmd in buff");
        return -1;
    }

    command.tag = 0xdead;
    packet_len = tlv_parse_put(&command, buff, BUFF_SIZE, &end_ptr);

    command.tag = 0xabcd;
    packet_len = tlv_parse_put(&command, buff, BUFF_SIZE, &end_ptr);


    while(1) {
        int rc = sendto(fd, buff, packet_len, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        log_debug(1, "send rc: %d", rc);
        usleep(500000);
    }
    close(fd);
    return 0;
}