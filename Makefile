
CC = gcc
CFLAGS = -Wall
INC =
LIB = -lpthread

nf9c: nf9c.o session.o
	${CC} nf9c.o session.o ${CFLAGS} ${INC} ${LIB} -o nf9c
nf9c.o: nf9c.c
	${CC} nf9c.c ${CFLAGS} ${INC} ${LIB} -c
session.o: session.c session.h
	${CC} session.c ${CFLAGS} ${INC} ${LIB} -c
clean:
	@rm -rf *.o nf9c
