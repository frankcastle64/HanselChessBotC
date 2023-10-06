# This is a make file for Windows using MinGW.

# Set the source directory
SRC_DIR := source/src
TEST_DIR := tests

HEADER_DIR := source/header
# Set the target directory
TARGET_DIR := bin
# Lists the source files
# SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(TEST_DIR)/*.c)
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Executable Name
TARGET := myprogram

# Target path
TARGET_PATH := $(TARGET_DIR)/$(TARGET).exe

# Compiler options
CC := gcc
# CFLAGS := -Wall -Ofast -I$(HEADER_DIR) -I$(TEST_DIR)
CFLAGS := -Wall -Ofast -I$(HEADER_DIR)

# Build target

all: create_directory $(TARGET) execute

build: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET_PATH) $^

clean:
	rd /q /s bin

create_directory:
	if not exist $(TARGET_DIR) mkdir $(TARGET_DIR)

execute:
	$(TARGET_PATH)

.PHONY: all build clean create_directory execute
