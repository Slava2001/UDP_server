#ifndef INCLUDE_TLV_PARSER_H
#define INCLUDE_TLV_PARSER_H

#include <stdint.h>

 #define MAX_DATA_LEN 64

typedef struct {
    uint16_t tag;
    uint16_t length;
    uint8_t data[MAX_DATA_LEN];
} command_t;

command_t tlv_parse_get(uint8_t *buff, uint32_t size, uint8_t **end_ptr);
int tlv_parse_put(command_t *command, uint8_t *buff, uint32_t size, uint8_t **end_ptr);

#endif // INCLUDE_TLV_PARSER_H