#include <hellfire.h>
#include "process_info.h"

uint8_t gausian(int8_t * buffer){
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
                        sum += ((int32_t)buffer[(i * 5) + j] * (int32_t)kernel[i][j]);
        mpixel = (int32_t)(sum / 159);

        return (uint8_t)mpixel;
}

void get_image_slice_from_center_point(int8_t * image, int x, int y, int border, int8_t * image_slice)
{
        int image_vertical, image_horizontal, slice_index = 0;

        int initial_x = x - border;
        int end_x = x + border;
        int initial_y = y - border;
        int end_y = y + border;

        for (image_vertical = initial_y; image_vertical < end_y; image_vertical++)
        {
                for (image_horizontal = initial_x; image_horizontal < end_x; image_horizontal++)
                {
                        image_slice[slice_index] = image[(image_vertical * TASK_IMAGE_SIZE) + image_horizontal];
                        slice_index++;
                }
        }
}

int8_t * do_gausian(int8_t * image, int8_t height, int8_t width)
{
        int image_vertical, image_horizontal = 0;
        int border = 2;

        for (image_vertical = border; image_vertical < (height - border); image_vertical++)
        {
                for (image_horizontal = border; image_horizontal < (width - border); image_horizontal++)
                {
                        int8_t image_slice[25];

                        get_image_slice_from_center_point(image, image_horizontal, image_vertical, border, image_slice);

                        image[(image_vertical * height) + image_horizontal] = gausian(image_slice);
                }
        }

        return image;
}


int32_t isqrt(int32_t a){
        int32_t i, rem = 0, root = 0, divisor = 0;

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

uint8_t sobel(int8_t * buffer){
        int32_t sum = 0, gx = 0, gy = 0;
        int8_t i, j;

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
                        gx += ((int32_t)buffer[(i * 3) + j] * (int32_t)kernelx[i][j]);
                        gy += ((int32_t)buffer[(i * 3) + j] * (int32_t)kernely[i][j]);
                }
        }

        sum = isqrt(gy * gy + gx * gx);

        if (sum > 255) sum = 255;
        if (sum < 0) sum = 0;

        return (uint8_t)sum;
}

int8_t * do_sobel(int8_t * image, int8_t height, int8_t width)
{
        int image_vertical, image_horizontal = 0;
        int border = 1;

        for (image_vertical = border; image_vertical < (height - border); image_vertical++)
        {
                for (image_horizontal = border; image_horizontal < (width - border); image_horizontal++)
                {
                        int8_t image_slice[9];

                        get_image_slice_from_center_point(image, image_horizontal, image_vertical, border, image_slice);

                        image[(image_vertical * height) + image_horizontal] = sobel(image_slice);
                }
        }

        return image;
}
