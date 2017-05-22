#include <hellfire.h>
#include <noc.h>

struct task_info {
        int id;
        int cpu;
        int port;
        int channel;
};

static struct task_info TASK_1 = { 1, 0, 60001, 1 };
static struct task_info TASK_2 = { 2, 1, 60002, 2 };
static struct task_info TASK_3 = { 3, 2, 60003, 3 };
static struct task_info TASK_4 = { 4, 3, 60004, 4 };
static struct task_info TASK_5 = { 5, 4, 60005, 5 };
static struct task_info TASK_6 = { 6, 5, 60006, 6 };
static struct task_info TASK_7 = { 7, 0, 60007, 7 };
static struct task_info TASK_8 = { 8, 0, 60008, 8 };
static struct task_info TASK_9 = { 9, 0, 60009, 9 };

static int send_message(struct task_info origin, struct task_info dest)
{
        int8_t buf[1500];
        uint16_t send_ack;

        send_ack = hf_sendack(dest.cpu, dest.port, buf, sizeof(buf), dest.channel, 500);
        if (send_ack)
                printf("task %d FAILED to transmit to task %d on CPU %d with error: %d\n", origin.id, dest.id, dest.cpu, send_ack);
        else
                printf("task %d transmitted to task %d on CPU %d\n", origin.id, dest.id, dest.cpu);

        return !send_ack;
}

static int check_and_send(int check, struct task_info origin, struct task_info dest)
{
        if (!check)
        {
                check = send_message(origin, dest);
                delay_ms(10);
        }

        return check;
}

static int receive_message(struct task_info origin)
{
        int8_t buf[1500];
        uint16_t cpu, receive_port, size, receive_ack;

        receive_ack = hf_recvack(&cpu, &receive_port, buf, &size, origin.channel);

        if (receive_ack)
                printf("hf_recvack(): error %d\n", receive_ack);
        else
        {
                printf("task %d received on port %d from CPU %d\n", origin.id, receive_port, cpu);
                return receive_port;
        }

        return !receive_ack;
}

static int check_port(int port, int intended_port)
{
        int is_correct = 0;

        if (port == intended_port)
                is_correct = 1;

        return is_correct;
}

void task1(void)
{
        int has_sended_to_task2 = 0, has_sended_to_task3 = 0, has_sended_to_task4 = 0,
            has_sended_to_task5 = 0, has_sended_to_task7 = 0;

        while (1)
        {
                has_sended_to_task2 = check_and_send(has_sended_to_task2, TASK_1, TASK_2);
                // has_sended_to_task3 = check_and_send(has_sended_to_task3, TASK_1, TASK_3);
                // has_sended_to_task4 = check_and_send(has_sended_to_task4, TASK_1, TASK_4);
                // has_sended_to_task5 = check_and_send(has_sended_to_task5, TASK_1, TASK_5);
                // has_sended_to_task7 = check_and_send(has_sended_to_task7, TASK_1, TASK_7);
        }
}

void task2(void)
{
        int has_received_from_task1 = 0;
        int has_sended_to_task6 = 0, has_sended_to_task7 = 0, has_sended_to_task8 = 0;

        while (1)
        {
                int received_channel = receive_message(TASK_2);

                has_received_from_task1 = check_port(received_channel, TASK_1.port);

                if (has_received_from_task1)
                {
                        has_sended_to_task6 = check_and_send(has_sended_to_task6, TASK_2, TASK_6);
                        // has_sended_to_task7 = check_and_send(has_sended_to_task7, TASK_2, TASK_7);
                        // has_sended_to_task8 = check_and_send(has_sended_to_task8, TASK_2, TASK_8);
                }
        }
}

void task3(void)
{
        int has_received_from_task1 = 0;
        int has_sended_to_task7 = 0, has_sended_to_task8 = 0;

        if (hf_comm_create(hf_selfid(), TASK_3.port, 0))
                panic(0xff);

        while (1)
        {
                int received_port = receive_message(TASK_3);

                has_received_from_task1 = check_port(received_port, TASK_1.port);

                if (has_received_from_task1)
                {
                        has_sended_to_task7 = check_and_send(has_sended_to_task7, TASK_3, TASK_7);
                        has_sended_to_task8 = check_and_send(has_sended_to_task8, TASK_3, TASK_8);
                }
        }
}

void task4(void)
{
        int has_received_from_task1 = 0;
        int has_sended_to_task8 = 0;

        if (hf_comm_create(hf_selfid(), TASK_4.port, 0))
                panic(0xff);

        while (1)
        {
                int received_port = receive_message(TASK_4);

                has_received_from_task1 = check_port(received_port, TASK_1.port);

                if (has_received_from_task1)
                {
                        has_sended_to_task8 = check_and_send(has_sended_to_task8, TASK_4, TASK_8);
                }
        }
}

void task5(void)
{
        int has_received_from_task1 = 0;
        int has_sended_to_task8 = 0;

        if (hf_comm_create(hf_selfid(), TASK_5.port, 0))
                panic(0xff);

        while (1)
        {
                int received_port = receive_message(TASK_5);

                has_received_from_task1 = check_port(received_port, TASK_1.port);

                if (has_received_from_task1)
                {
                        has_sended_to_task8 = check_and_send(has_sended_to_task8, TASK_5, TASK_8);
                }
        }
}

void task6(void)
{
        int has_received_from_task2 = 0;
        int has_sended_to_task9 = 0;

        while (1)
        {
                int received_port = receive_message(TASK_6);

                has_received_from_task2 = check_port(received_port, TASK_2.port);

                if (has_received_from_task2)
                {
                        has_sended_to_task9 = check_and_send(has_sended_to_task9, TASK_6, TASK_9);
                }
        }
}

void task7(void)
{
        int has_received_from_task1 = 0, has_received_from_task2 = 0, has_received_from_task3 = 0;
        int has_sended_to_task9 = 0;

        if (hf_comm_create(hf_selfid(), TASK_7.port, 0))
                panic(0xff);

        while (1)
        {
                int received_port = receive_message(TASK_7);

                has_received_from_task1 = check_port(received_port, TASK_1.port);
                has_received_from_task2 = check_port(received_port, TASK_2.port);
                has_received_from_task3 = check_port(received_port, TASK_3.port);

                if (has_received_from_task1 && has_received_from_task2 && has_received_from_task3)
                {
                        has_sended_to_task9 = check_and_send(has_sended_to_task9, TASK_7, TASK_9);
                }
        }
}

void task8(void)
{
        int has_received_from_task2 = 0, has_received_from_task3 = 0, has_received_from_task4 = 0,
            has_received_from_task5 = 0;
        int has_sended_to_task9 = 0;

        if (hf_comm_create(hf_selfid(), TASK_8.port, 0))
                panic(0xff);

        while (1)
        {
                int received_port = receive_message(TASK_8);

                has_received_from_task2 = check_port(received_port, TASK_2.port);
                has_received_from_task3 = check_port(received_port, TASK_3.port);
                has_received_from_task4 = check_port(received_port, TASK_4.port);
                has_received_from_task5 = check_port(received_port, TASK_5.port);

                if (has_received_from_task2 &&
                    has_received_from_task3 &&
                    has_received_from_task4 &&
                    has_received_from_task5)
                {
                        has_sended_to_task9 = check_and_send(has_sended_to_task9, TASK_8, TASK_9);
                }
        }
}

void task9(void)
{
        int has_received_from_task6 = 0, has_received_from_task7 = 0, has_received_from_task8 = 0;

        printf("Listening on port %d\n", TASK_9.port);

        while (1)
        {
                int received_port = receive_message(TASK_9);

                has_received_from_task6 = check_port(received_port, TASK_6.port);
                has_received_from_task7 = check_port(received_port, TASK_7.port);
                has_received_from_task8 = check_port(received_port, TASK_8.port);

                if (has_received_from_task6 && has_received_from_task7 && has_received_from_task8)
                {
                        printf("Network completed.\n");
                }
        }
}

void app_main(void)
{
        if (hf_cpuid() == TASK_1.cpu)
        {
                int task_id = hf_spawn(task1, 4, 1, 4, "task1", 4096);
                hf_comm_create(task_id, TASK_1.port, 0);
        }
        if (hf_cpuid() == TASK_2.cpu)
        {
                int task_id = hf_spawn(task2, 4, 1, 4, "task2", 4096);
                hf_comm_create(task_id, TASK_2.port, 0);
        }
        // if (hf_cpuid() == TASK_3.cpu)
        // {
        //         hf_spawn(task3, 0, 0, 0, "task3", 4096);
        // }
        // if (hf_cpuid() == TASK_4.cpu)
        // {
        //         hf_spawn(task4, 0, 0, 0, "task4", 4096);
        // }
        // if (hf_cpuid() == TASK_5.cpu)
        // {
        //         hf_spawn(task5, 0, 0, 0, "task5", 4096);
        // }
        if (hf_cpuid() == TASK_6.cpu)
        {
                int task_id = hf_spawn(task6, 4, 1, 4, "task6", 4096);
                hf_comm_create(task_id, TASK_6.port, 0);

        }
        // if (hf_cpuid() == TASK_7.cpu)
        // {
        //         hf_spawn(task7, 0, 0, 0, "task7", 4096);
        // }
        // if (hf_cpuid() == TASK_8.cpu)
        // {
        //         hf_spawn(task8, 0, 0, 0, "task8", 4096);
        // }
        if (hf_cpuid() == TASK_9.cpu)
        {
                int task_id = hf_spawn(task9, 4, 1, 4, "task9", 4096);
                hf_comm_create(task_id, TASK_9.port, 0);
        }
}
