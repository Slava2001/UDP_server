#ifndef INCLUDE_COMMAND_PROC_H
#define INCLUDE_COMMAND_PROC_H

#include <poll.h>
#include "tlv_parser.h"

int init_commands();
int get_command_fd(const command_t *c, struct pollfd *fd);
int command_proc(const command_t *c);

#endif // INCLUDE_COMMAND_PROC_H