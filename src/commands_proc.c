#include "commands_proc.h"
#include "util.h"

int show_data_as_string(const command_t *c);
int show_data_as_byte_array(const command_t *c);

int command_proc(const command_t *c) {
    switch (c->tag)
    {
    case 0xbeef:
        return show_data_as_string(c);
    case 0xdead:
        return show_data_as_byte_array(c);
    }
    log_error("Uncknown command. Command tag 0x%x", c->tag);
    return -1;
}

int show_data_as_string(const command_t *c) {
    log_raw("recv command: tag: 0x%04x len: 0x%04x data: %s\n", c->tag, c->length, c->data);
    return 0;
}

int show_data_as_byte_array(const command_t *c) {
    log_raw("recv command: tag: 0x%04x len: 0x%04x data: ", c->tag, c->length);
    for (int i = 0; i < c->length; ++i) {
        log_raw("%02X ", c->data[i]);
    }
    log_raw("\n");
    return 0;
}