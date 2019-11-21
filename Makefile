CFLAGS  := $(INCLUDE_DIR)
CFLAGS  += -g 
CFLAGS  += -Wall 
SRC := *.c
SRC += ./xui/*.c
#SRC += ./bit/bitmap.c ./bit/linux_fb.c
INC =-I.
INC +=-I./xui/
 
all:
	$(CC) $(INC) -g -Wall $(SRC) -lm -lpthread -o bin/xshow
clean:
	rm -f bin/*
	
	
	#-lpthread