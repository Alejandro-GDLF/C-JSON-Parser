# Directories
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin
HEADERS_DIR=./headers

# Headers of the project
HEADERS=$(wildcard $(HEADERS_DIR)/*.h)

# Source files
SRC=$(wildcard $(SRC_DIR)/*.c)

# Tests
UNIT_TESTS=$(wildcard tests/unit_tests/*.c)
TESTS=$(wildcard tests/tests/*.c)
TESTS_BIN=$(patsubst tests/tests/%.c, bin/%, $(TESTS))

# Compiler options
CC=gcc
CFLAGS=-g -Wall -Wextra -iquote $(HEADERS_DIR)
PRODFLAGS= -Wl,--strip-all -O2

SUBMITNAME=c_json_parser