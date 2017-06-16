#include <hellfire.h>
#include <noc.h>
#include "slave.h"
#include "image_filter.h"
#include "process_info.h"

void slave_task(void)
{
        int8_t send_msg_buffer[MESSAGE_SIZE];
        int8_t receive_msg_buffer[MESSAGE_SIZE];
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
                                printf("\nRecebeu o receive_msg_buffer (%d)\n", size);
                                int8_t message_index = get_message_index(receive_msg_buffer);
                                printf("Message id (%d)\n", message_index);
                                int8_t * message_content = get_content(receive_msg_buffer);

                                message_content = do_gausian(message_content, TASK_IMAGE_SIZE, TASK_IMAGE_SIZE);
                                message_content = do_sobel(message_content, TASK_IMAGE_SIZE, TASK_IMAGE_SIZE);

                                set_message_index(send_msg_buffer, message_index);
                                set_content(send_msg_buffer, message_content);

                                if (hf_cpuid() >= 7)
                                {
                                        printf("\nWaiting...\n");
                                        delay_ms(40);
                                }

                                val = hf_sendack(0, 5000, send_msg_buffer, sizeof(send_msg_buffer), hf_cpuid(), 3000);
                                if (val) {
                                        printf("hf_sendack(): error %d\n", val);
                                } else {
                                        printf("\nprocessador respondeu!");
                                }
                        }
                }
        }
}
