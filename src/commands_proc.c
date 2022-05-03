#include "commands_proc.h"
#include "util.h"
#include <poll.h>
#include <sys/timerfd.h>
#include <unistd.h>

struct command_desc {
    uint16_t tag;
    int fd;
    int (*init_command_fd)(struct command_desc *cd);
    int (*get_command_pollfd)(struct command_desc *cd, struct pollfd *fd);
    int (*command_proc)(struct command_desc *cd, const command_t *c);
};

int init_show_data_as_string_fd(struct command_desc *cd);
int get_show_data_as_string_pollfd(struct command_desc *cd, struct pollfd *fd);
int show_data_as_string(struct command_desc *cd, const command_t *c);

int init_show_data_as_byte_array_fd(struct command_desc *cd);
int get_show_data_as_byte_array_pollfd(struct command_desc *cd, struct pollfd *fd);
int show_data_as_byte_array(struct command_desc *cd, const command_t *c);

int init_timer_fd(struct command_desc *cd);
int get_timer_pollfd(struct command_desc *cd, struct pollfd *fd);
int waiting_timer(struct command_desc *cd, const command_t *c);

#define COMMAND_COUNT 3
struct command_desc commands[COMMAND_COUNT] = {
    { 0xbeef, -1, init_show_data_as_string_fd,     get_show_data_as_string_pollfd,     show_data_as_string     },
    { 0xdead, -1, init_show_data_as_byte_array_fd, get_show_data_as_byte_array_pollfd, show_data_as_byte_array },
    { 0xabcd, -1, init_timer_fd,                   get_timer_pollfd,                   waiting_timer           }
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

int get_command_pollfd(const command_t *c, struct pollfd *fd) {
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        if (c->tag == commands[i].tag) {
            return commands[i].get_command_pollfd(&commands[i], fd);
        }
    }
    log_error("Failed to get command pollfd. Unknown command tag: 0x%04x", c->tag);
    return -1;
}

int get_command_fd(const command_t *c) {
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        if (c->tag == commands[i].tag) {
            return commands[i].fd;
        }
    }
    log_error("Failed to get command fd. Unknown command tag: 0x%04x", c->tag);
    return -1;
}

int command_proc(const command_t *c) { 
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        if (c->tag == commands[i].tag) {
            return commands[i].command_proc(&commands[i], c);
        }
    }
    log_error("Failed to proc command. Unknown command tag: 0x%04x", c->tag);
    return -1;
}

// commands func

int init_show_data_as_string_fd(struct command_desc *cd) {
    cd->fd = 2; //stderr
    return 0;
}

int get_show_data_as_string_pollfd(struct command_desc *cd, struct pollfd *fd) {
    log_debug(1, "Enter");
    fd->fd = cd->fd;
    fd->events = POLLOUT;
    fd->revents = 0;
    return 0;
}

int show_data_as_string(struct command_desc *cd, const command_t *c) {
    if (cd) {} // fix unused
    log_raw("show_data_as_string: "GREEN"tag: 0x%04x"NONE" len: 0x%04x data: %s\n", c->tag, c->length, c->data);
    return 0;
}


int init_show_data_as_byte_array_fd(struct command_desc *cd) {
    log_debug(1, "Enter");
    cd->fd = 1; //stdout
    return 0;
}

int get_show_data_as_byte_array_pollfd(struct command_desc *cd, struct pollfd *fd) {
    log_debug(1, "Enter");
    fd->fd = cd->fd;
    fd->events = POLLOUT;
    fd->revents = 0;
    return 0;
}

int show_data_as_byte_array(struct command_desc *cd, const command_t *c) {
    if (cd) {} // fix unused
    log_raw("show_data_as_byte_array: "GREEN"tag: 0x%04x"NONE" len: 0x%04x data: ", c->tag, c->length);
    for (int i = 0; i < c->length; ++i) {
        log_raw("%02X ", c->data[i]);
    }
    log_raw("\n");
    return 0;
}


int init_timer_fd(struct command_desc *cd) {
    log_debug(1, "Enter");
    
    cd->fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (cd->fd == -1) {
        log_perror("Create timer fd.");
		return -1;
	}
    
    return 0;
}

int get_timer_pollfd(struct command_desc *cd, struct pollfd *fd) {
    log_debug(1, "Enter");
    if (fd->fd == cd->fd) {
        return 0; // timer already work
    }

    struct itimerspec ts;
	ts.it_interval.tv_sec = 1;
	ts.it_interval.tv_nsec = 0;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;

	if (timerfd_settime(cd->fd, 0, &ts, NULL) < 0) {
		log_perror("set time.");
        close(cd->fd);
		return -1;
	}
    
    fd->fd = cd->fd;
    fd->events = POLLIN;
    fd->revents = 0;
    return 0;
}

int waiting_timer(struct command_desc *cd, const command_t *c) {
    if (c) {} // fix unused
    char buff[8];
    read(cd->fd, buff, 8);
    log_raw("waiting_timer: "GREEN"tag: 0x%04x"NONE" len: 0x%04x data: Timer event\n", c->tag, c->length);
    return 0;
}
