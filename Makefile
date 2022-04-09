
CC = gcc
CFLAGS = -pthread -std=gnu11 -Wall
INC =
LIB =

nf9p: nf9p.o nf9r.o buffer.o socket.o watch.o
	${CC} nf9p.o nf9r.o buffer.o socket.o watch.o ${CFLAGS} ${INC} ${LIB} -o nf9p

nf9p.o: nf9p.c
	${CC} nf9p.c ${CFLAGS} ${INC} ${LIB} -c

nf9r.o: nf9r.c
	${CC} nf9r.c ${CFLAGS} ${INC} ${LIB} -c

buffer.o: buffer.c
	${CC} buffer.c ${CFLAGS} ${INC} ${LIB} -c

socket.o: socket.c
	${CC} socket.c ${CFLAGS} ${INC} ${LIB} -c

watch.o: watch.c
	${CC} watch.c ${CFLAGS} ${INC} ${LIB} -c

clean:
	@rm -rf *.o nf9p
