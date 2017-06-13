#include <hellfire.h>
#include "process_info.h"

uint16_t * get_content(uint16_t * message)
{
        return &message[1];
}

uint8_t * get_process_type(uint16_t * message)
{
        return message[0];
}
