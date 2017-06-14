#include <hellfire.h>

int8_t gausian(int8_t * buffer){
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

        return (int8_t)mpixel;
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
                        image_slice[slice_index] = image[(image_vertical * 32) + image_horizontal];
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
