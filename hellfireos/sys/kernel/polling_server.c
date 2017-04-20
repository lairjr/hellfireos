#include <hal.h>
#include <queue.h>
#include <kernel.h>
#include <panic.h>
#include <polling_server.h>

int32_t get_aperiodic_task()
{
		int32_t queue_count;

		queue_count = hf_queue_count(krnl_aperiodic_queue);
		if (queue_count == 0)
				return polling_server_id;
		krnl_task = hf_queue_remhead(krnl_aperiodic_queue);

		if (krnl_task->state != TASK_BLOCKED && krnl_task->capacity_rem > 0 && krnl_task->id) {
				--krnl_task->capacity_rem;
				krnl_task->apjobs++;
				if (hf_queue_addtail(krnl_aperiodic_queue, krnl_task)) panic(PANIC_CANT_PLACE_APERIODIC);
		}

		return krnl_task->id;
}

void polling_server(void)
{
		int32_t rc;

		for (;; ) {
				hf_schedlock(1);
				krnl_task = &krnl_tcb[krnl_current_task];
				rc = setjmp(krnl_task->task_context);
				if (rc) {
						return;
				}
				if (krnl_task->state == TASK_RUNNING)
						krnl_task->state = TASK_READY;
				if (krnl_task->pstack[0] != STACK_MAGIC)
						panic(PANIC_STACK_OVERFLOW);

				krnl_current_task = get_aperiodic_task();
				krnl_task->state = TASK_RUNNING;
				if (krnl_current_task != 1) {
						krnl_pcb.coop_cswitch++;
				}
		#if KERNEL_LOG >= 1
				dprintf("\n%d %d %d %d %d ", krnl_current_task, krnl_task->period, krnl_task->capacity, krnl_task->deadline, (uint32_t)_read_us());
		#endif
				hf_schedlock(0);
				_restoreexec(krnl_task->task_context, 1, krnl_current_task);
		}
}
