SRC_DIR = src
BIN_DIR = bin

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c $(SRC_DIR)/parser.c $(SRC_DIR)/interpreter.c

TARGET = $(BIN_DIR)/compiler

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)
	gcc -o $(TARGET) $(SRCS) -I$(SRC_DIR)

clean:
	rm -rf $(BIN_DIR)
