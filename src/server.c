#include <unistd.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>

#include "server.h"
#include "util.h"

int init_server(server_t *server, struct pollfd *fd) {
    assert(server);
    server->fd = fd;
    server->fd->fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server->fd->fd == -1) {
        log_perror("Failed to create socket.");
        return -1;
    }
    
    struct sockaddr_un server_addr = { };

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_SOCKET_PATH, 108);

    if (unlink(SERVER_SOCKET_PATH) && errno != 2 /*No such file or directory*/) {
        log_perror("Failed to unlink socket file.");
        return -1;
    }

    if (bind(server->fd->fd, (const struct sockaddr *)&server_addr, sizeof(server_addr))) {
        log_perror("Failed to bind socket.");
        deinit_server(server);
        return -1;
    }

    server->fd->events = POLLIN;
    server->fd->revents = 0;

    log_debug(1, "Server started...");
    return 0;
}

int recv_dgram(server_t *server, uint8_t *buff, uint32_t size) {
    assert(server);
    server->fd->revents = 0;
    int rc = recv(server->fd->fd, buff, size, 0);
    if (rc < 0) {
        log_perror("Failed to recv msg.");
        return -1;
    }
    if ((uint32_t)rc >= size) {
        log_error("Failed to recv msg. msg to long");
        return -1;
    }
    log_debug(1, "recv len: %d", rc);
    return rc;
}

void deinit_server(server_t *server) {
    assert(server);
    close(server->fd->fd);
    server->fd->fd = -1;
}