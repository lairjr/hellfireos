#include <hellfire.h>
#include <noc.h>
#include "master.h"
#include "slave.h"

void app_main(void)
{
        if (hf_cpuid() == 0) {
                hf_spawn(master_task, 0, 0, 0, "master", 4096);
        } else {
                hf_spawn(slave_task, 0, 0, 0, "receiver", 4096);
        }
}
