
CC = gcc
CFLAGS = -pthread -std=gnu11 -Wall -DINFO
INC =
LIB =

main: main.o nf9r.o buffer.o udp.o watch.o
	${CC} main.o nf9r.o buffer.o udp.o watch.o ${CFLAGS} ${INC} ${LIB} -o nf9

main.o: main.c
	${CC} main.c ${CFLAGS} ${INC} ${LIB} -c

nf9r.o: nf9r.c
	${CC} nf9r.c ${CFLAGS} ${INC} ${LIB} -c

buffer.o: buffer.c
	${CC} buffer.c ${CFLAGS} ${INC} ${LIB} -c

udp.o: udp.c
	${CC} udp.c ${CFLAGS} ${INC} ${LIB} -c

watch.o: watch.c
	${CC} watch.c ${CFLAGS} ${INC} ${LIB} -c

clean:
	@rm -rf *.o nf9
