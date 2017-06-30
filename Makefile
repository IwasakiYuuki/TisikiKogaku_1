.PHONY:outcreate libcreate clean

CC = gcc
SOUCE_DIR = ./src/
BIN_DIR = ./bin/
#OBJ = $(shell ls $(wildcard $(addprefix $(SOUCE_DIR),*.c)))
OBJ = $(shell ls $(SOUCE_DIR))
HAEDER_DIR = ./include/
LIB_DIR = ./lib/
LIB =
CREATE_LOG = echo "[*]create files [$@] from [$^]."

outcreate: $(addprefix $(BIN_DIR),$(OBJ:.c=.out))

libcreate: $(addprefix $(LIB_DIR),$(LIB))


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
