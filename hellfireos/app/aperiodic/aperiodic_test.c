#include <hellfire.h>

void task(void){
		int32_t jobs, id;

		id = hf_selfid();
		for(;; ) {
				jobs = hf_jobs(id);
				printf("\n%s Id: %d Total Capacity: %d Capacity Rem: %d", hf_selfname(), id, hf_selfcapacity(), hf_selfcapacity_rem());
				while (jobs == hf_jobs(id)) ;
		}
}

void aperiodic(void) {
		int32_t jobs, id;

		id = hf_selfid();
		for(;; ) {
				jobs = hf_jobs(id);
				printf("\n%s Id: %d Total Capacity: %d Capacity Rem: %d", hf_selfname(), id, hf_selfcapacity(), hf_selfcapacity_rem());
				while (jobs == hf_jobs(id)) ;
		}
}

void trigger_aperiodic(void) {
		int32_t jobs, id;

		id = hf_selfid();
		for(;; ) {
				// int r = (random() % 140) + 60;
				delay_ms(200);
				hf_spawn(aperiodic, 0, 2, 0, "aperiodic a", 2048);
		}
}

void app_main(void) {
		hf_spawn(task, 4, 1, 4, "task a", 2048);
		hf_spawn(task, 8, 2, 8, "task b", 2048);
		hf_spawn(task, 12, 1, 12, "task c", 2048);
		hf_spawn(trigger_aperiodic, 0, 0, 0, "aperiodic trigger", 2048);
}
