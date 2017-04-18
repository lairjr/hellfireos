image:
	docker build -t hellfire .

bash:
	docker run -v ~/My_Projects/hellfireos/hellfireos:/hellfireos -t -i hellfire /bin/bash

simulator:
	docker run -v ~/My_Projects/hellfireos/hellfireos:/hellfireos -w /hellfireos/usr/sim/hf_risc_sim/ -t -i hellfire gcc -O2 hf_risc_sim.c -o hf_risc_sim

single-core-image:
	docker run -v ~/My_Projects/hellfireos/hellfireos:/hellfireos -w /hellfireos/platform/single_core -t -i hellfire make image

single-core: simulator single-core-image
	docker run -v ~/My_Projects/hellfireos/hellfireos:/hellfireos -w /hellfireos/platform/single_core -t -i hellfire ../../usr/sim/hf_risc_sim/hf_risc_sim image.bin
