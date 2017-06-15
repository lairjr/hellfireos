#include <hellfire.h>
#include <noc.h>
#include "master.h"
#include "process_info.h"
#include "image.h"

void distribute_tasks()
{
        int32_t y, x, image_index;
        int32_t i = 1;
        int16_t val;
        int8_t buffer[MESSAGE_SIZE];

        for (y = 0; y < TASK_IMAGE_SIZE; y++)
        {
                image_index = 0;
                for (x = 0; x < TASK_IMAGE_SIZE; x++)
                {
                        buffer[i] = image[(y * image_width) + image_index];
                        i++;
                        image_index++;
                }
        }

        val = hf_sendack(1, 5000, buffer, sizeof(buffer), 1, 500);
        if (val) {
                printf("hf_sendack(): error %d\n", val);
        } else {
                printf("enviou para outra task");
        }
}

void receive_tasks()
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
                        printf("Received");
                }
        }
}

void master_task(void)
{
        uint8_t *img;
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

                delay_ms(200);

                receive_tasks();

                free(img);

                panic(0);
        }
}
