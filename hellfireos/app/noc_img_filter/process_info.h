#define TASK_IMAGE_SIZE 32
#define MESSAGE_SIZE (TASK_IMAGE_SIZE * TASK_IMAGE_SIZE) + 1
#define GAUSIAN 1
#define SOBEL 2

int8_t * get_content(int8_t * message);
int8_t get_message_index(int8_t * message);
void set_message_index(int8_t * message, int8_t type);
void set_content(int8_t * message, int8_t * content);
