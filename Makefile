FOLDER_PATH = ~/My_Projects/hellfireos/hellfireos

image:
	docker build -t hellfire .

bash:
	docker run -v $(FOLDER_PATH):/hellfireos -t -i hellfire /bin/bash

simulator:
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/sim/hf_risc_sim/ -t -i hellfire gcc -O2 hf_risc_sim.c -o hf_risc_sim

single-core-image:
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/platform/single_core -t -i hellfire make image

single-core: simulator single-core-image
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/platform/single_core --name singlecore --rm -t -i hellfire ../../usr/sim/hf_risc_sim/hf_risc_sim image.bin

dump-single-core: simulator single-core-image
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/platform/single_core --name singlecore --rm -t -i hellfire ../../usr/sim/hf_risc_sim/hf_risc_sim image.bin log_file.txt

stop:
	docker stop $$(docker ps -f "name=singlecore" --format="{{.ID}}")
