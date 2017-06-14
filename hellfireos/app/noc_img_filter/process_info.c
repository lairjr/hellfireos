#include <hellfire.h>
#include "process_info.h"

int8_t * get_content(int8_t * message)
{
        return &message[1];
}

int8_t get_process_type(int8_t * message)
{
        return message[0];
}
