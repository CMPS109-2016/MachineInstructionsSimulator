CC = g++
INC_PATH = includes/
SOCK_INC_PATH = socket/include/
CFLAGS = -std=c++14 -Wfatal-errors -g

BIN = bin/

##############CORE##############

CORE_SRC = src/mis-core/
CORE_SRC_NAME = $(wildcard $(CORE_SRC)*.cpp)
CORE_OBJ = $(notdir $(patsubst %.cpp, %.o, $(CORE_SRC_NAME)))

bin/$(CORE_OBJ): $(CORE_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CORE_SRC_NAME) -I $(INC_PATH)
	@mv $(CORE_OBJ) $(BIN)

##############INSTRUCTIONS##############

INST_SRC = src/mis-instructions/
INST_SRC_NAME = $(wildcard $(INST_SRC)*.cpp)
INST_OBJ = $(notdir $(patsubst %.cpp, %.o, $(INST_SRC_NAME)))

bin/$(INST_OBJ): $(INST_SRC_NAME) $(CORE_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(INST_SRC_NAME) -I $(INC_PATH)
	@mv $(INST_OBJ) $(BIN)

##############SOCKET##############

SOC_SRC_NAME = $(wildcard socket/*.cpp)
SOC_OBJ = $(notdir $(patsubst %.cpp, %.o, $(SOC_SRC_NAME)))
SOC_INCLUDE = socket/include/

bin/$(SOC_OBJ): $(SOC_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SOC_SRC_NAME) -I $(SOC_INCLUDE)
	@mv $(SOC_OBJ) $(BIN)

##############CLIENT##############

CLIENT_SRC_NAME = $(wildcard src/mis-client/*.cpp)
CLIENT_OBJ = $(notdir $(patsubst %.cpp, %.o, $(CLIENT_SRC_NAME)))

bin/$(CLIENT_OBJ): $(CLIENT_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC_NAME) -I $(INC_PATH) $(SOC_INCLUDE)
	@mv $(CLIENT_OBJ) $(BIN)

##############SERVER##############

SERVER_SRC_NAME = $(wildcard src/mis-server/*.cpp)
SERVER_OBJ = $(notdir $(patsubst %.cpp, %.o, $(SERVER_SRC_NAME)))

bin/$(SERVER_OBJ): $(SERVER_SRC_NAME)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c $(SERVER_SRC_NAME) -I $(INC_PATH) $(SOC_INCLUDE)
	@mv $(SERVER_OBJ) $(BIN)
