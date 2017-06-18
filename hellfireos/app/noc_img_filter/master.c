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

int8_t * img;

void print_img()
{
        int x;
        int break_line = 0;
        printf("\n\n");
        for (x = 0; x < (image_width * image_height); x++)
        {
                printf("0x%x, ", img[x]);
                break_line++;
                if (break_line == 16)
                {
                        printf("\n");
                        break_line = 0;
                }
        }
        printf("\n\n");
}

void replace_on_img(int pos_x, int pos_y, int8_t * content)
{
        int end_x = pos_x + TASK_IMAGE_SIZE;
        if (end_x > image_width)
        {
                end_x = image_width;
        }

        int end_y = pos_y + TASK_IMAGE_SIZE;
        if (end_y > image_height)
        {
                end_y = image_height;
        }
        int content_index = 0;

        for (pos_y; pos_y < end_y; pos_y++)
        {
                int temp_pos_x = pos_x;
                for (temp_pos_x; temp_pos_x < end_x; temp_pos_x++)
                {
                        int index = (pos_y * image_width) + temp_pos_x;
                        img[index] = content[content_index];
                        content_index++;
                }
        }
}

void receive_tasks()
{
        int received_tasks = 0;

        while (received_tasks < 5)
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

                                printf("%d %d\n", pos_x, pos_y);

                                replace_on_img(pos_x, pos_y, message_content);
                        }
                }
        }
}

int8_t * create_buffer(int pos_x, int pos_y, int8_t * buffer)
{
        int32_t i = 1;
        int end_x = pos_x + TASK_IMAGE_SIZE;
        if (end_x > image_width)
        {
                end_x = image_width;
        }

        int end_y = pos_y + TASK_IMAGE_SIZE;
        if (end_y > image_height)
        {
                end_y = image_height;
        }

        printf("%d %d até %d %d\n", pos_y, pos_x, end_y, end_x);

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
                for (x = 0; x < image_width; x = x + TASK_IMAGE_SIZE)
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
                                if (cpu > 5) {
                                        cpu = 1;
                                        printf("waiting to receive packets\n");
                                        delay_ms(200);
                                        receive_tasks();
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
                img = (int8_t *) malloc(image_width * image_height);
                if (img == NULL) {
                        printf("\nmalloc() failed!\n");
                        for(;; ) ;
                }

                time = _readcounter();

                distribute_tasks();

                print_img();

                time = _readcounter() - time;

                printf("\n\nDone in %d clock cycles.\n\n", time);

                free(img);

                panic(0);
        }
}
