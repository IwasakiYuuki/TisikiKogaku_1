.PHONY:clean all out

CC = gcc
OBJ = 3J04_kadai3.c
SOUCE_DIR = ./src/
BIN_DIR = ./bin/
HAEDER_DIR = ./include/
LIB_DIR = ./lib/
LIB =
CREATE_LOG = echo "[*]create files [$@] from [$^]"

all: $(addprefix $(BIN_DIR),$(OBJ:.c=.out)) clean

libcreat:
	@make ${LIB}


$(addprefix $(BIN_DIR),$(OBJ:.c=.out)):$(addprefix $(BIN_DIR),$(OBJ:.c=.o))
	@${CC} $< -o $@
	@${CREATE_LOG}

$(addprefix $(BIN_DIR),%.o):$(addprefix $(SOUCE_DIR),%.c)
	@${CC} -c $< -o $@
	@${CREATE_LOG}

.lib.o:
	@${CC} -shered $< -o $@
	@${CREATE_LOG}

clean:
	@rm -rf $(wildcard $(addprefix $(BIN_DIR),*.o))

$(addprefix $(BIN_DIR),$(OBJ:.c=.o)):$(addprefix $(SOUCE_DIR),$(OBJ))
${LIB}:${LIB:.lib=.o}
