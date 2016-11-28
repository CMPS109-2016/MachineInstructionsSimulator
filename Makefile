CC = g++
INC_PATH = includes/
SOCK_INC_PATH = socket/include/
CFLAGS = -std=c++14 -Wfatal-errors -g

BIN = bin/

.PHONY
	core
	instructions
	socket
	client
	server
all:
	core
	instructions
	socket
	client
	server

##############CORE##############

CORE_SRC = src/mis-core/
CORE_SRC_NAME = $(wildcard $(CORE_SRC)*.cpp)
CORE_OBJ = $(notdir $(patsubst %.cpp, %.o, $(CORE_SRC_NAME)))

core: $(CORE_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CORE_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)

##############INSTRUCTIONS##############

INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(notdir $(patsubst %.cpp, %.o, $(INST_SRC_NAME)))

instructions: core
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(INST_OBJ) $(BIN)

##############SOCKET##############

SOC_SRC_NAME = $(wildcard socket/*.cpp)
SOC_OBJ = $(notdir $(patsubst %.cpp, %.o, $(SOC_SRC_NAME)))

socket: $(SOC_SRC_NAME)
	@mkdir -p $(BIN)
    $(CC) $(CFLAGS) -c $(SOC_SRC_NAME) -I socket/include
    @mv $(SOC_OBJ) $(BIN)

##############CLIENT##############

CLIENT_SRC_NAME = $(wildcard src/mis-client/*.cpp)
CLIENT_OBJ = $(notdir $(patsubst %.cpp, %.o, $(CLIENT_SRC_NAME)))

client: socket instruction $(CLIENT_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC_NAME) -I $(INC_PATH)
	@mv $(CLIENT_OBJ) $(BIN)

##############SERVER##############

SERVER_SRC_NAME = $(wildcard src/mis-server/*.cpp)
SERVER_OBJ = $(notdir $(patsubst %.cpp, %.o, $(SERVER_SRC_NAME)))

server: socket instruction $(SERVER_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SERVER_SRC_NAME) -I $(INC_PATH)
	@mv $(SERVER_OBJ) $(BIN)

# $(EXEC): $(OB J)
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