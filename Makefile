CC = g++
INC_PATH = includes/
SOCK_INC_PATH = socket/include/
CFLAGS = -std=c++14 -Wfatal-errors -g

BIN = bin/


INST_INCLUDE = include/mis-instructions/
INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(patsubst %.cpp, %.o, $(INST_SRC_NAME))

.PHONY: all core instructions

all:
	core
	instructions

CORE_SRC = src/mis-core/
CORE_SRC_NAME = $(wildcard $(CORE_SRC)*.cpp)
CORE_OBJ = $(patsubst %.cpp, %.o, $(CORE_SRC_NAME))

core:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CORE_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)


INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(patsubst %.cpp, %.o, $(INST_SRC_NAME))

instructions:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(INST_OBJ) $(BIN)


#$(EXEC): $(OB J)
#	@mkdir -p $(OBJ_PATH)
#	@mkdir -p $(EXEC_PATH)
#	$(CC) $(CFLAGS) -o $@ $(CORE_OBJ) -I $(INC_PATH)
#	@mv $(CORE_OBJ) $(OBJ_PATH)
#	@cp $(EXEC) $(EXEC_PATH)

#$(CORE_OBJ):
#	$(CC) $(CFLAGS) -c $(addprefix $(SRC_PATH), $(patsubst %.o, %.cpp, $@)) -I $(INC_PATH) $(SOCK_INC_PATH)

#wipe:
#	rm -f $(OBJ) $(EXEC) $(EXEC_PATH)*

#clean:
#	rm -f $(OBJ) $(EXEC)