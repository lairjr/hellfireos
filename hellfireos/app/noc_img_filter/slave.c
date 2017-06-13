#define TASK_IMAGE_SIZE 32
#include <hellfire.h>
#include <noc.h>
#include "slave.h"

void slave_task(void)
{
        uint16_t * message_content;
        uint16_t message_buffer[TASK_IMAGE_SIZE * TASK_IMAGE_SIZE];
        uint8_t message_type;
        uint16_t cpu, task, size;
        int16_t val;
        int32_t i;

        if (hf_comm_create(hf_selfid(), 5000, 0))
                panic(0xff);

        while (1) {
                i = hf_recvprobe();
                if (i >= 0) {
                        val = hf_recvack(&cpu, &task, message_buffer, &size, i);
                        if (val)
                                printf("hf_recvack(): error %d\n", val);
                        else
                        {
                                // do_gausian(message_buffer, 32, 32);
                                printf("Recebeu o message_buffer (%d)\n", size);
                                message_type = get_process_type(message_buffer);
                                printf("Message type (%d)\n", message_type);
                                message_content = get_content(message_buffer);
                                int a;
                                for (a = 0; a < 1024; a++) {
                                        printf("%x \n", message_content[a]);

                                }
                                val = hf_sendack(0, 5000, message_buffer, sizeof(message_buffer), 1, 500);
                                if (val) {
                                        printf("hf_sendack(): error %d\n", val);
                                } else {
                                        printf("processador respondeu!");
                                }
                        }
                }
        }
}
