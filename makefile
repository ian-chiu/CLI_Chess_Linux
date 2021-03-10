# our final target 'chess' executable is in build directory
TARGET_EXEC := chess 

CC = gcc
CFLAGS = -Wall -g

BUILD_DIR = ./build
SRC_DIR = ./src

# Find all the C files we want to compile
SRCS = $(shell find $(SRC_DIR) -name *.c)

# Use math library
LIBS = -lm

# Predefine a "CWD" macro so that we can get the makefile directory inside the program (in persistance.h)
PREDEFINED_MACROS = -DCWD='"$(shell pwd)"'

# String substitution for every C file.
# As an example, ./src/hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Build step for C source
# make a 'build' directory if it is not exsist
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(PREDEFINED_MACROS) $(CFLAGS) -c $< -o $@ 

clean:
	rm -r $(BUILD_DIR)