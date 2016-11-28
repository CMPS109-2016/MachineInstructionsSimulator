CC = g++
INC_PATH = includes/
SOCK_INC_PATH = socket/include/
CFLAGS = -std=c++14 -Wfatal-errors -g

BIN = bin/

CORE_INCLUDE = includes/mis-core/
CORE_SRC = src/mis-core/
CORE_SRC_NAME = $(wildcard $(CORE_SRC)*.cpp)
CORE_OBJ = $(patsubst %.cpp, %.o, $(CORE_SRC_NAME))

INST_INCLUDE = include/mis-instructions/
INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(patsubst %.cpp, %.o, $(INST_SRC_NAME))

INST_INCLUDE = include/mis-instructions/
INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(patsubst %.cpp, %.o, $(INST_SRC_NAME))


EXEC_PATH = bin/
EXEC = misvm


.PHONY: all core

all:
	$(CORE_OBJ)
	@echo $(CORE_SRC)
	@echo "\n"
	@echo $(CORE_SRC_NAME)
	@echo "\n"
	@echo $(CORE_OBJ)
	@echo "\n"
	@echo $(CORE_INCLUDE)
#	${MAKE} $(EXEC) || ${MAKE} wipe


$(CORE_OBJ): $(INST_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)
	
core:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)
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