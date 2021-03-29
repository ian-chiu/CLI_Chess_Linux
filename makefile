# our final target 'chess' executable is in build directory
TARGET_EXEC = chess 

CC = gcc
CFLAGS = -Wall -g

BUILD_DIR = ./build
SRC_DIR = ./src
OBJ_DIR = $(BUILD_DIR)/objects

# Find all the C files we want to compile
SRCS = $(shell find $(SRC_DIR) -name *.c)

LIBS = -L./lib -lm -lev

INCLUDES = -I./include

# Predefine a "CWD" (current working directory) macro so that we can get the makefile directory inside the program (in fileManagement.h)
PREDEFINED_MACROS = -DCWD='"$(shell pwd)"'

# String substitution for every C file.
# As an example, ./src/hello.cpp turns into ./build/hello.cpp.o
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Build step for C source
# make a 'build' or 'build/object' directory if it is not exsist
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(PREDEFINED_MACROS) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

clean:
	rm -r $(BUILD_DIR)

run:
	./build/chess