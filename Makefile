
CC = gcc
CFLAGS = -Wall -DINFO
INC =
LIB =

nf9p: nf9p.o nf9r.o
	${CC} nf9p.o nf9r.o ${CFLAGS} ${INC} ${LIB} -o nf9p

nf9p.o: nf9p.c
	${CC} nf9p.c ${CFLAGS} ${INC} ${LIB} -c

nf9r.o: nf9r.c
	${CC} nf9r.c ${CFLAGS} ${INC} ${LIB} -c

clean:
	@rm -rf *.o nf9p
