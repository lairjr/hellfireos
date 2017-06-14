#define TASK_IMAGE_SIZE 32
#include <hellfire.h>
#include <noc.h>
#include "slave.h"
#include "image_filter.h"
#include "process_info.h"

void slave_task(void)
{
        int8_t send_msg_buffer[MESSAGE_SIZE];
        int8_t receive_msg_buffer[MESSAGE_SIZE];
        uint8_t message_type;
        uint16_t cpu, task, size;
        int16_t val;
        int32_t i;

        if (hf_comm_create(hf_selfid(), 5000, 0))
                panic(0xff);

        while (1) {
                i = hf_recvprobe();
                if (i >= 0) {
                        val = hf_recvack(&cpu, &task, receive_msg_buffer, &size, i);
                        if (val)
                                printf("hf_recvack(): error %d\n", val);
                        else
                        {
                                printf("Recebeu o receive_msg_buffer (%d)\n", size);
                                message_type = get_process_type(receive_msg_buffer);
                                printf("Message type (%d)\n", message_type);
                                int8_t * message_content = get_content(receive_msg_buffer);

                                message_content = do_gausian(message_content, 32, 32);

                                set_process_type(send_msg_buffer, GAUSIAN);
                                set_content(send_msg_buffer, message_content);

                                val = hf_sendack(0, 5000, send_msg_buffer, sizeof(send_msg_buffer), 1, 500);
                                if (val) {
                                        printf("hf_sendack(): error %d\n", val);
                                } else {
                                        printf("processador respondeu!");
                                }
                        }
                }
        }
}
