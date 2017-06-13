APP_DIR = $(SRC_DIR)/$(APP)

app: kernel
	$(CC) $(CFLAGS) \
		$(APP_DIR)/process_info.c \
		$(APP_DIR)/master.c \
		$(APP_DIR)/slave.c \
		$(APP_DIR)/noc_img_filter.c
