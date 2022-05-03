#include "commands_proc.h"
#include "util.h"
#include "poll.h"

struct command_desc {
    uint16_t tag;
    int fd;
    int (*init_command_fd)(struct command_desc *cd);
    int (*get_command_fd)(struct command_desc *cd, struct pollfd *fd);
    int (*command_proc)(const command_t *c);
};

int init_show_data_as_string_fd(struct command_desc *cd);
int get_show_data_as_string_fd(struct command_desc *cd, struct pollfd *fd);
int show_data_as_string(const command_t *c);
int init_show_data_as_byte_array_fd(struct command_desc *cd);
int get_show_data_as_byte_array_fd(struct command_desc *cd, struct pollfd *fd);
int show_data_as_byte_array(const command_t *c);

#define COMMAND_COUNT 2
struct command_desc commands[COMMAND_COUNT] = {
    { 0xbeef, -1, init_show_data_as_string_fd,     get_show_data_as_string_fd,     show_data_as_string     },
    { 0xdead, -1, init_show_data_as_byte_array_fd, get_show_data_as_byte_array_fd, show_data_as_byte_array },
};

int init_commands() {
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        int rc = commands[i].init_command_fd(&commands[i]);
        if (rc) {
            log_error("Failed to init command. Command tag: 0x%04x", commands[i].tag);
            return -1;
        }
    }
    return 0;
}

int get_command_fd(const command_t *c, struct pollfd *fd) {
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        if (c->tag == commands[i].tag) {
            return commands[i].get_command_fd(&commands[i], fd);
        }
    }
    log_error("Failed to get command fd. Unknown command tag: 0x%04x", c->tag);
    return -1;
}

int command_proc(const command_t *c) { 
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        if (c->tag == commands[i].tag) {
            return commands[i].command_proc(c);
        }
    }
    log_error("Failed to proc command. Unknown command tag: 0x%04x", c->tag);
    return -1;
}





int init_show_data_as_string_fd(struct command_desc *cd) {
    cd->fd = 2; //stderr
    return 0;
}

int get_show_data_as_string_fd(struct command_desc *cd, struct pollfd *fd) {
    log_debug(1, "Enter");
    fd->fd = cd->fd;
    fd->events = POLLOUT;
    fd->revents = 0;
    return 0;
}

int show_data_as_string(const command_t *c) {
    log_raw("show_data_as_string: tag: 0x%04x len: 0x%04x data: %s\n", c->tag, c->length, c->data);
    return 0;
}

int init_show_data_as_byte_array_fd(struct command_desc *cd) {
    log_debug(1, "Enter");
    cd->fd = 1; //stdout
    return 0;
}

int get_show_data_as_byte_array_fd(struct command_desc *cd, struct pollfd *fd) {
    fd->fd = cd->fd;
    fd->events = POLLOUT;
    fd->revents = 0;
    return 0;
}

int show_data_as_byte_array(const command_t *c) {
    log_raw("show_data_as_byte_array: tag: 0x%04x len: 0x%04x data: ", c->tag, c->length);
    for (int i = 0; i < c->length; ++i) {
        log_raw("%02X ", c->data[i]);
    }
    log_raw("\n");
    return 0;
}