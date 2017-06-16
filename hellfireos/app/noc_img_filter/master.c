#include <hellfire.h>
#include <noc.h>
#include "master.h"
#include "process_info.h"
#include "image.h"

struct message
{
        int8_t id;
        int pos_x;
        int pos_y;
};

struct message messages[500];

uint8_t * img;

void replace_on_img(int pos_x, int pos_y, int8_t * content)
{
        int end_x = pos_x + TASK_IMAGE_SIZE;
        int end_y = pos_y + TASK_IMAGE_SIZE;
        int content_index = 0;

        for (pos_y; pos_y < end_y; pos_y++)
        {
                for (pos_x; pos_x < end_x; pos_x++)
                {
                        img[(pos_y * image_width) + pos_x] = content[content_index];
                        content_index++;
                }
        }
}

void receive_tasks()
{
        int received_tasks = 0;

        while (received_tasks < 8)
        {
                uint32_t i = 0;
                uint16_t cpu, task, size;
                int8_t message[MESSAGE_SIZE];
                int16_t val;

                i = hf_recvprobe();
                if (i >= 0) {
                        val = hf_recvack(&cpu, &task, message, &size, i);
                        if (val)
                                printf("hf_recvack(): error %d\n", val);
                        else
                        {
                                printf("\nReceived from CPU %d \n", cpu);
                                received_tasks++;

                                int8_t message_index = get_message_index(message);
                                int8_t * message_content = get_content(message);

                                int pos_x = messages[message_index].pos_x;
                                int pos_y = messages[message_index].pos_y;

                                replace_on_img(pos_x, pos_y, message_content);
                        }
                }
        }
}

int8_t * create_buffer(int pos_x, int pos_y, int8_t * buffer)
{
        int32_t i = 1;
        int end_y = pos_y + TASK_IMAGE_SIZE;
        int end_x = pos_x + TASK_IMAGE_SIZE;

        for (pos_y; pos_y < end_y; pos_y++)
        {
                int image_index = pos_x;
                for (pos_x; pos_x < end_x; pos_x++)
                {
                        buffer[i] = image[(pos_y * image_width) + image_index];
                        i++;
                        image_index++;
                }
        }
}

void distribute_tasks()
{
        int x, y, message_index = 1;
        int16_t cpu = 1;

        for (y = 0; y < image_height; y = y + TASK_IMAGE_SIZE)
        {
                for (x = 0; x < image_height; x = x + TASK_IMAGE_SIZE)
                {
                        int16_t val;
                        int8_t buffer[MESSAGE_SIZE];

                        buffer[0] = message_index;
                        create_buffer(x, y, buffer);

                        val = hf_sendack(cpu, 5000, buffer, sizeof(buffer), message_index, 500);

                        if (val) {
                                printf("hf_sendack(): error %d\n", val);
                        } else {
                                printf("enviou para outra task no CPU %d\n", cpu);
                                messages[message_index].id = message_index;
                                messages[message_index].pos_x = x;
                                messages[message_index].pos_y = y;
                                message_index++;

                                cpu++;
                                if (cpu > 8) {
                                        cpu = 1;
                                        printf("waiting to receive packets\n");
                                        delay_ms(200);
                                        receive_tasks();

                                        int x;
                                        printf("\n\n");
                                        for (x = 0; x < (image_width * image_height); x++)
                                        {
                                                printf("0x%x, ", img[x]);
                                        }
                                        printf("\n\n");
                                }
                        }
                }
        }
}

void master_task(void)
{
        uint32_t time;

        if (hf_comm_create(hf_selfid(), 5000, 0))
                panic(0xff);

        while(1) {
                img = (uint8_t *) malloc(image_width * image_height);
                if (img == NULL) {
                        printf("\nmalloc() failed!\n");
                        for(;; ) ;
                }

                time = _readcounter();

                distribute_tasks();

                time = _readcounter() - time;

                free(img);

                panic(0);
        }
}
