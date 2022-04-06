
CC = gcc
CFLAGS = -Wall -DINFO
INC =
LIB =

nf9p: nf9p.o
	${CC} nf9p.o ${CFLAGS} ${INC} ${LIB} -o nf9p

nf9p.o: nf9p.c
	${CFLAGS} += -lpthread
	${CC} nf9p.c ${CFLAGS} ${INC} ${LIB} -c

clean:
	@rm -rf *.o nf9p
