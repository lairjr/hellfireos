#define TASK_IMAGE_SIZE 32
#include <hellfire.h>
#include <noc.h>
#include "slave.h"
#include "image.h"

uint8_t gausian(int8_t buffer[5][5]) {
        int32_t sum = 0, mpixel;
        uint8_t i, j;

        int16_t kernel[5][5] =  {
                {2, 4, 5, 4, 2},
                {4, 9, 12, 9, 4},
                {5, 12, 15, 12, 5},
                {4, 9, 12, 9, 4},
                {2, 4, 5, 4, 2}
        };
        for (i = 0; i < 5; i++)
                for (j = 0; j < 5; j++)
                        sum += ((int32_t)buffer[i][j] * (int32_t)kernel[i][j]);
        mpixel = (int32_t)(sum / 159);

        return (int8_t)mpixel;
}

uint32_t isqrt(uint32_t a){
        uint32_t i, rem = 0, root = 0, divisor = 0;

        for (i = 0; i < 16; i++) {
                root <<= 1;
                rem = ((rem << 2) + (a >> 30));
                a <<= 2;
                divisor = (root << 1) + 1;
                if (divisor <= rem) {
                        rem -= divisor;
                        root++;
                }
        }
        return root;
}

uint8_t sobel(uint8_t buffer[3][3]){
        int32_t sum = 0, gx = 0, gy = 0;
        uint8_t i, j;

        int16_t kernelx[3][3] = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
        };
        int16_t kernely[3][3] = {
                {-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}
        };
        for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                        gx += ((int32_t)buffer[i][j] * (int32_t)kernelx[i][j]);
                        gy += ((int32_t)buffer[i][j] * (int32_t)kernely[i][j]);
                }
        }

        sum = isqrt(gy * gy + gx * gx);

        if (sum > 255) sum = 255;
        if (sum < 0) sum = 0;

        return (uint8_t)sum;
}

void do_gausian(int8_t *img, int32_t width, int32_t height){
        int32_t i, j, k, l;
        int8_t image_buf[5][5];

        for(i = 0; i < height; i++) {
                if (i > 1 || i < height-1) {
                        for(j = 0; j < width; j++) {
                                if (j > 1 || j < width-1) {
                                        for (k = 0; k < 5; k++)
                                                for(l = 0; l < 5; l++)
                                                        image_buf[k][l] = image[(((i + l-2) * width) + (j + k-2))];

                                        img[((i * width) + j)] = gausian(image_buf);
                                }else{
                                        img[((i * width) + j)] = image[((i * width) + j)];
                                }
                        }
                }
        }
}

void do_sobel(uint8_t *img, int32_t width, int32_t height){
        int32_t i, j, k, l;
        uint8_t image_buf[3][3];

        for(i = 0; i < height; i++) {
                if (i > 0 || i < height) {
                        for(j = 0; j < width; j++) {
                                if (j > 0 || j < width) {
                                        for (k = 0; k < 3; k++)
                                                for(l = 0; l < 3; l++)
                                                        image_buf[k][l] = image[(((i + l-1) * width) + (j + k-1))];

                                        img[((i * width) + j)] = sobel(image_buf);
                                }else{
                                        img[((i * width) + j)] = image[((i * width) + j)];
                                }
                        }
                }
        }
}

void slave_task(void)
{
        int8_t buffer[TASK_IMAGE_SIZE * TASK_IMAGE_SIZE];
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
                                do_gausian(buffer, 32, 32);
                                printf("fez o buffer\n");
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
