
SRC_FILES := main.c prompt.c input.c warp.c peek.c pastevents.c system.c proclore.c seek.c neonate.c iMan.c activities.c ping.c ctrl.c redirection.c


OBJ_FILES := $(SRC_FILES:.c=.o)


TARGET := myprogram


CC := gcc
CFLAGS := -g -Wall
LDFLAGS := -lcurl  # Add -lcurl here to link with the libcurl library




%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET): $(OBJ_FILES)
	$(CC)  $^ -o $@  $(LDFLAGS)

clean:
	rm -f $(OBJ_FILES) $(TARGET)
