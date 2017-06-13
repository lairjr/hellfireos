#define TASK_IMAGE_SIZE 32
#include <hellfire.h>
#include <noc.h>
#include "master.h"
#include "process_info.h"
#include "image.h"

void distribute_gausin_tasks(int32_t width, int32_t height)
{
        int32_t y, x;
        int32_t i = 0;
        int16_t val;
        int16_t buffer[TASK_IMAGE_SIZE * TASK_IMAGE_SIZE];

        for (y = 0; y < height; y++)
        {
                for (x = 0; x < width; x++)
                {
                        buffer[i] = image[i];
                        i++;
                }
        }

        printf("Stack BUFFER!\n");
        int a;
        for (a = 0; a < (TASK_IMAGE_SIZE * TASK_IMAGE_SIZE); a++) {
                printf("%x \n", buffer[a]);
        }
        val = hf_sendack(1, 5000, buffer, sizeof(buffer), 1, 500);
        if (val) {
                printf("hf_sendack(): error %d\n", val);
        } else {
                printf("enviou para outra task");
        }
}

void master_task(void)
{
        uint32_t i = 0;
        uint8_t *img;
        int16_t message[MESSAGE_SIZE];
        int8_t * message_content;
        int8_t message_type;
        int16_t val;
        uint16_t cpu, task, size;
        uint32_t time;

        if (hf_comm_create(hf_selfid(), 5000, 0))
                panic(0xff);

        while(1) {
                img = (uint8_t *) malloc(312 * 234);
                if (img == NULL) {
                        printf("\nmalloc() failed!\n");
                        for(;; ) ;
                }

                time = _readcounter();

                distribute_gausin_tasks(32, 32);

                time = _readcounter() - time;

                delay_ms(500);
                i = hf_recvprobe();
                if (i >= 0) {
                        printf("TESTE %d\n", i);
                        val = hf_recvack(&cpu, &task, message, &size, i);
                        if (val)
                                printf("hf_recvack(): error %d\n", val);
                        else
                        {
                                printf("RECEBEU %d!\n", size);
                                //message_content = get_content(message);
                                int a;
                                for (a = 0; a < 1024; a++) {
                                        printf("%x \n", message[a]);
                                }
                        }
                }

                free(img);

                panic(0);
        }
}
