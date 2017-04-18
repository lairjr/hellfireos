image:
	docker build -t hellfire .

bash:
	docker run -v ~/My_Projects/hellfireos/hellfireos:/hellfireos -t -i hellfire /bin/bash
