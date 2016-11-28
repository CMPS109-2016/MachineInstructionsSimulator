CC = g++
INC_PATH = includes/
SOCK_INC_PATH = socket/include/
CFLAGS = -std=c++14 -Wfatal-errors -g

BIN = bin/

.PHONY:
	dump
	core
	inst
	socket
	client
	server

##############CORE##############

CORE_SRC = src/mis-core/
CORE_SRC_NAME = $(wildcard $(CORE_SRC)*.cpp)
CORE_OBJ = $(notdir $(patsubst %.cpp, %.o, $(CORE_SRC_NAME)))
CORE_BIN_OBJ = $(addprefix $(BIN),$(CORE_OBJ))

core:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CORE_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)

$(CORE_BIN_OBJ): $(CORE_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CORE_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)

##############INSTRUCTIONS##############

INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(notdir $(patsubst %.cpp, %.o, $(INST_SRC_NAME)))
INST_BIN_OBJ = $(addprefix $(BIN),$(INST_OBJ))

inst:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(INST_OBJ) $(BIN)

$(INST_BIN_OBJ): $(INST_SRC_NAME) $(CORE_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(INST_OBJ) $(BIN)

##############SOCKET##############

SOC_SRC_NAME = $(wildcard socket/*.cpp)
SOC_OBJ = $(notdir $(patsubst %.cpp, %.o, $(SOC_SRC_NAME)))
SOC_INCLUDE = socket/include/
SOC_BIN_OBJ = $(addprefix $(BIN), $(SOC_OBJ))

socket:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SOC_SRC_NAME) -I $(SOC_INCLUDE)
	@mv $(SOC_OBJ) $(BIN)

$(SOC_BIN_OBJ): $(SOC_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SOC_SRC_NAME) -I $(SOC_INCLUDE)
	@mv $(SOC_OBJ) $(BIN)

##############CLIENT##############

CLIENT_SRC_NAME = $(wildcard src/mis-client/*.cpp)
CLIENT_OBJ = $(notdir $(patsubst %.cpp, %.o, $(CLIENT_SRC_NAME)))
CLIENT_BIN_OBJ = $(addprefix $(BIN), $(CLIENT_OBJ))

client:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC_NAME) -I $(INC_PATH) -I  $(SOC_INCLUDE)
	@mv $(CLIENT_OBJ) $(BIN)

$(CLIENT_BIN_OBJ): $(CLIENT_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC_NAME) -I $(INC_PATH) -I  $(SOC_INCLUDE)
	@mv $(CLIENT_OBJ) $(BIN)

##############SERVER##############

SERVER_SRC_NAME = $(wildcard src/mis-server/*.cpp)
SERVER_OBJ = $(notdir $(patsubst %.cpp, %.o, $(SERVER_SRC_NAME)))
SERVER_BIN_OBJ = $(addprefix $(BIN), $(SERVER_OBJ))

server:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SERVER_SRC_NAME) -I $(INC_PATH) -I $(SOC_INCLUDE)
	@mv $(SERVER_OBJ) $(BIN)

$(SERVER_BIN_OBJ): $(SERVER_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SERVER_SRC_NAME) -I $(INC_PATH) -I $(SOC_INCLUDE)
	@mv $(SERVER_OBJ) $(BIN)

dump:
	@echo $(CORE_BIN_OBJ)
	@echo $(INST_BIN_OBJ)
	@echo $(SOC_BIN_OBJ)
	@echo $(CLIENT_BIN_OBJ)
	@echo $(SERVER_BIN_OBJ)
