CFLAGS          := -Wall -O2 -ggdb -pie -fpie

LLSRCS          := linkedlist.[ch]

all: poc_linkedlist

poc_linkedlist: ${LLSRCS} poc_linkedlist.c

clean:
	rm -f poc_linkedlist

