#include "tlv_parser.h"

#include <assert.h>
#include <string.h>

command_t tlv_parse_get(uint8_t *buff, uint32_t size, uint8_t **end_ptr) {
    assert(buff);
    command_t command;
    
    if (!*end_ptr) {
        *end_ptr = buff;
    }

    memcpy((char*)&command.tag, *end_ptr, sizeof(command.tag));
    memcpy((char*)&command.length, *end_ptr + sizeof(command.tag), sizeof(command.length));
    memcpy((char*)&command.data, *end_ptr + sizeof(command.tag) + sizeof(command.length), command.length);
   
    *end_ptr += sizeof(command.tag) + sizeof(command.length) + command.length;
    if (*end_ptr >= buff+size) {
        *end_ptr = NULL;
    }
    
    return command;
}

int tlv_parse_put(command_t *command, uint8_t *buff, uint32_t size, uint8_t **end_ptr) {
    assert(buff);
    assert(command);
    assert(command->length <= MAX_DATA_LEN);

    if (!*end_ptr) {
        *end_ptr = buff;
    }

    uint32_t command_size = sizeof(command->tag) + sizeof(command->length) + command->length;
    if ((*end_ptr + command_size) > (buff+size)) {
        return -1;
    }

    memcpy(*end_ptr, (char*)&command->tag, sizeof(command->tag));
    memcpy(*end_ptr + sizeof(command->tag), (char*)&command->length, sizeof(command->length));
    memcpy(*end_ptr + sizeof(command->tag) + sizeof(command->length), command->data, command->length);
    
    *end_ptr += command_size;
    return *end_ptr - buff;
}