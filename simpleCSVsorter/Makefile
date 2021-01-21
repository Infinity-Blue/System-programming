CC	=	gcc

CFLAGS = -Wall -Werror

all : simpleCSVsorter


simpleCSVsorter : simpleCSVsorter.c mergesort.c simpleCSVsorter.h

	$(CC) $(CFLAGS) simpleCSVsorter.c mergesort.c -o simpleCSVsorter -lm

clean :

	rm -f simpleCSVsorter
