#define TASK_IMAGE_SIZE 32
#include <hellfire.h>
#include <noc.h>
#include "master.h"

void distribute_gausin_tasks(uint8_t *img, int32_t width, int32_t height)
{
        int32_t y, x;
        int32_t i = 0;
        int16_t val;

        for (y = 0; y < height; y++)
        {
                for (x = 0; x < width; x++)
                {
                        int8_t buffer[TASK_IMAGE_SIZE * TASK_IMAGE_SIZE];

                        buffer[i] = img[((y * width) + x)];
                        if (i == (TASK_IMAGE_SIZE * TASK_IMAGE_SIZE) - 1)
                        {
                                i = 0;
                                val = hf_sendack(1, 5000, buffer, sizeof(buffer), 1, 500);
                                if (val) {
                                        printf("hf_sendack(): error %d\n", val);
                                } else {
                                        printf("enviou para outra task");
                                }
                        }
                        i++;
                }
        }
}

void master_task(void)
{
        uint32_t i = 0;
        uint8_t *img;
        int8_t buffer[TASK_IMAGE_SIZE * TASK_IMAGE_SIZE];
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

                distribute_gausin_tasks(img, 32, 32);

                time = _readcounter() - time;

                delay_ms(500);
                i = hf_recvprobe();
                if (i >= 0) {
                        printf("TESTE %d\n", i);
                        val = hf_recvack(&cpu, &task, buffer, &size, i);
                        if (val)
                                printf("hf_recvack(): error %d\n", val);
                        else
                        {
                                printf("Recebeu %d!\n", size);
                                int b;
                                for (b = 0; b < size; b++)
                                {
                                        printf("0x%x, ", buffer[b]);
                                }
                        }
                }

                free(img);

                panic(0);
        }
}
