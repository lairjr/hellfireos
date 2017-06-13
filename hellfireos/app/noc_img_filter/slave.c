#define TASK_IMAGE_SIZE 32
#include <hellfire.h>
#include <noc.h>
#include "slave.h"

void slave_task(void)
{
        int16_t buffer[TASK_IMAGE_SIZE * TASK_IMAGE_SIZE];
        uint16_t cpu, task, size;
        int16_t val;
        int32_t i;

        if (hf_comm_create(hf_selfid(), 5000, 0))
                panic(0xff);

        while (1) {
                i = hf_recvprobe();
                if (i >= 0) {
                        val = hf_recvack(&cpu, &task, buffer, &size, i);
                        if (val)
                                printf("hf_recvack(): error %d\n", val);
                        else
                        {
                                // do_gausian(buffer, 32, 32);
                                printf("Recebeu o buffer (%d)\n", size);
                                int a;
                                for (a = 0; a < size; a++) {
                                        printf("%x \n", buffer[a]);

                                }
                                val = hf_sendack(0, 5000, buffer, sizeof(buffer), 1, 500);
                                if (val) {
                                        printf("hf_sendack(): error %d\n", val);
                                } else {
                                        printf("processador respondeu!");
                                }
                        }
                }
        }
}
