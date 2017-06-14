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

mpsoc-simulator:
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/sim/mpsoc_sim -t -i hellfire make noc_3x3

mpsoc-images:
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/platform/noc_3x3 -t -i hellfire make images

mpsoc: mpsoc-simulator mpsoc-images
	cp hellfireos/platform/noc_3x3/*.bin hellfireos/usr/sim/mpsoc_sim/objects/
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/sim/mpsoc_sim -t -i --rm hellfire ./mpsoc_sim 3 s

compile-image-tools:
	rm -rf hellfireos/usr/image_processing/create_bmp hellfireos/usr/image_processing/create_image
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/image_processing -t -i hellfire gcc create_bmp.c -o create_bmp
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/image_processing -t -i hellfire gcc create_image.c -o create_image

image-bytes: compile-image-tools
	rm -rf hellfireos/usr/image_processing/create_bmp hellfireos/usr/image_processing/filter_image.h
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/image_processing -t -i hellfire ./create_image processing.bmp > $(FOLDER_PATH)/usr/image_processing/image.h

image-bmp: compile-image-tools
	rm -rf hellfireos/usr/image_processing/output.bmp
	docker run -v $(FOLDER_PATH):/hellfireos -w /hellfireos/usr/image_processing -t -i hellfire ./create_bmp

stop:
	docker stop $$(docker ps -f "name=singlecore" --format="{{.ID}}")
