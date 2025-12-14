CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS := -lpthread

SRC_DIR := src
OBJ_DIR := build
BIN := a.out

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
