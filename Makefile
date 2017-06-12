.PHONY:clean all

CC = gcc
OBJ = src/3J04_kadai3.c
HAEDER_DIR = 
LIB_DIR =
LIB =
CREATE_LOG = echo "[*]create files [$@] from [$^]\n"

outcreat:
	@make ${OBJ:.c=.out}
	@make clean

libcreat:
	@make ${LIB}


${OBJ:.c=.out}:${OBJ:.c=.o}
	@${CC} $< -o $@
	@${CREATE_LOG}

.o.c:
	@${CC} -c $<
	@${CREATE_LOG}

.lib.o:
	@${CC} -shered $< -o $@
	@${CREATE_LOG}

clean:
	@rm -rf *.obj

${OBJ:.c=.o}:${OBJ}
${LIB}:${LIB:.lib=.o}
