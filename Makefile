.PHONY:clean all out

CC = gcc
SOUCE_DIR = ./src/
BIN_DIR = ./bin/
OBJ_DIR = ./obj/
OBJ = $(shell ls $(SOUCE_DIR))
HAEDER_DIR = ./include/
LIB_DIR = ./lib/
LIB =
CREATE_LOG = echo "[*]create files [$@] from [$^]."

all: $(addprefix $(BIN_DIR),$(OBJ:.c=.out))

libcreat:
	@make ${LIB}


$(addprefix $(BIN_DIR),%.out):$(addprefix $(BIN_DIR),%.o)
	@${CC} $< -o $@
	@${CREATE_LOG}

$(addprefix $(BIN_DIR),%.o):$(addprefix $(SOUCE_DIR),%.c)
	@${CC} -c $< -o $@

.lib.o:
	@${CC} -shered $< -o $@
	@${CREATE_LOG}

clean:
	@rm -rf $(wildcard $(addprefix $(BIN_DIR),*.o))
	@echo "[*]delete files [$(wildcard $(addprefix $(BIN_DIR),*.o))]."

${LIB}:${LIB:.lib=.o}
