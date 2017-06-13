#define TASK_IMAGE_SIZE 32
#define MESSAGE_SIZE (TASK_IMAGE_SIZE * TASK_IMAGE_SIZE) + 1
#define GAUSIAN 1
#define SOBEL 2

uint16_t * get_content(uint16_t * message);
uint8_t * get_process_type(uint16_t * message);
