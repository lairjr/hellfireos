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
                                printf("Message id (%d)\n", message_type);
                                int8_t * message_content = get_content(receive_msg_buffer);

                                message_content = do_gausian(message_content, TASK_IMAGE_SIZE, TASK_IMAGE_SIZE);
                                message_content = do_sobel(message_content, TASK_IMAGE_SIZE, TASK_IMAGE_SIZE);

                                set_process_type(send_msg_buffer, GAUSIAN);
                                set_content(send_msg_buffer, message_content);

                                int x;
                                for (x = 0; x < MESSAGE_SIZE; x++)
                                {
                                        printf("%x ", send_msg_buffer[x]);
                                }
                        }
                }
        }
}
