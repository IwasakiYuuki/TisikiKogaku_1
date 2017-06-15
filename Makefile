.PHONY:clean all

CC = gcc
OBJ = 3J04_kadai3.c
SOUCE_DIR = ./src
BIN_DIR = ./bin
HAEDER_DIR = ./include
LIB_DIR = ./lib
LIB =
CREATE_LOG = echo "[*]create files [$@] from [$^]\n"

outcreat:
	@make ${OBJ:.c=.out}
	@make clean

libcreat:
	@make ${LIB}


$(BIN_DIR)/${OBJ:.c=.out}:$(BIN_DIR)/${OBJ:.c=.o}
	@${CC} $< -o $^
	@${CREATE_LOG}

$(BIN_DIR)/%.o:$(SOUSE_DIR)/%.c
	@${CC} -c $<
	@${CREATE_LOG}

.lib.o:
	@${CC} -shered $< -o $@
	@${CREATE_LOG}

clean:
	@rm -rf *.obj

$(BIN_DIR)/${OBJ:.c=.o}:$(SOUSE_DIR)/${OBJ}
${LIB}:${LIB:.lib=.o}
