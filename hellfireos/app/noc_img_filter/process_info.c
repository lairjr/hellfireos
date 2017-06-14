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

void set_process_type(int8_t * message, int8_t type)
{
        message[0] = type;
}

void set_content(int8_t * message, int8_t * content)
{
        int x, message_index;

        message_index = 1;

        for (x = 0; x < 1024; x++)
        {
                message[message_index] = content[x];
                message_index++;
        }
}
