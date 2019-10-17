CFLAGS  := $(INCLUDE_DIR)
CFLAGS  += -g 
CFLAGS  += -Wall 
SRC := *.c
#SRC += ./tree/*.c

all:
	$(CC) -g -Wall $(SRC) -lm -o bin/xshow
clean:
	rm -f bin/*
	
	
	#-lpthread