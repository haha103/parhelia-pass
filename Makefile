CXX = g++
#CXX = clang++
CXX_FLAGS = -g -Wall
#CXX_FLAGS = -O2 -Wall
INC_PATH = -I./include
LIB_PATH = -L./lib
LIBS = -lsqlite3 -lcryptopp

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

EXEC = $(BIN_DIR)/parhelia_pass


$(EXEC) : $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(INC_PATH) $(LIB_PATH) $(LIBS) $(OBJECTS) -o $@

$(BUILD_DIR)/main.o : $(SRC_DIR)/main.cpp
	$(CXX) $(CXX_FLAGS) $(INC_PATH) -c $< -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	$(CXX) $(CXX_FLAGS) $(INC_PATH) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)

pp:
	echo $(SOURCES)
	echo $(OBJECTS)
	echo $(BUILD_DIR)
