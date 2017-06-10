.PHONY:clean all

CC = gcc
OBJ = 3J04_kadai.c
HAEDER_DIR = 
LIB_DIR =
LIB =

outcreat:
	make ${OBJ:.c=.out}
	make clean

libcreat:
	make ${LIB}


${OBJ:.c=.out}:${OBJ:.c=.o}
	${CC} $< -o $@

.o.c:
	${CC} -c $<

.lib.o:
	${CC} -shered $< -o $@

clean:
	rm -rf *.obj

${OBJ:.c=.o}:${OBJ}
${LIB}:${LIB:.lib=.o}
