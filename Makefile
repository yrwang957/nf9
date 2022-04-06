
CC = gcc
CFLAGS = -Wall
INC =
LIB = -lpthread

nf9c: nf9c.o session.o bufferManagement.o outputProcess.o
	${CC} nf9c.o session.o bufferManagement.o outputProcess.o ${CFLAGS} ${INC} ${LIB} -o nf9c

nf9c.o: nf9c.c
	${CC} nf9c.c ${CFLAGS} ${INC} ${LIB} -c

session.o: session.c session.h
	${CC} session.c ${CFLAGS} ${INC} ${LIB} -c

bufferManagement.o: bufferManagement.c bufferManagement.h
	${CC} bufferManagement.c ${CFLAGS} ${INC} ${LIB} -c

outputProcess.o: outputProcess.c outputProcess.h
	${CC} outputProcess.c ${CFLAGS} ${INC} ${LIB} -c

clean:
	@rm -rf *.o nf9c
