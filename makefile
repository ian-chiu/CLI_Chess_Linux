# our final target 'chess' executable is in build directory
TARGET_EXEC := chess 

CC = gcc
CFLAGS = -Wall -g

BUILD_DIR = ./build
SRC_DIR = ./src

# Find all the C and C++ files we want to compile
SRCS = $(shell find $(SRC_DIR) -name *.c)

# Use math library
LIBS = -lm

# String substitution for every C file.
# As an example, ./src/hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC) : $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Build step for C source
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -r $(BUILD_DIR)