#ifndef INCLUDE_SERVER_H
#define INCLUDE_SERVER_H

#include <poll.h>
#include <stdint.h>

#define SERVER_PORT 31337
#define SERVER_SOCKET_PATH "/tmp/udp_server_socket"

typedef struct {
    struct pollfd fd;
} server_t;

int init_server(server_t *server);
int recv_dgram(server_t *server, uint8_t *buff, uint32_t size);
void deinit_server(server_t *server);

#endif // INCLUDE_SERVER_H
