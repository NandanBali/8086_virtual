# Compiler and compilation flags
CC = g++ 
CFLAGS = -Wall -Wextra -g

# Directories
SRC_DIR = src
BUILD_DIR = build

# The final executable name
TARGET = $(BUILD_DIR)/8086_emu

# Find all .cpp files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Generate a list of object files (.o) in the build directory
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile .cpp files into .o files
# The '| $(BUILD_DIR)' is an order-only prerequisite, ensuring the directory exists
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR)

# Tell Make that these aren't actual files
.PHONY: all clean
