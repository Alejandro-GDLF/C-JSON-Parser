CC=gcc
CFLAGS=-g -Wall

HEADERS=headers/c_json_parser.h headers/c_json_parser_types.h
SRC=src/c_json_parser.c
OBJ=obj/c_json_parser.o

UNIT_TESTS=$(wildcard tests/*.c)

SUBMITNAME=c_json_parser

all: $(OBJ)

$(OBJ): $(HEADERS) $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $@


unit: bin/unit_tests

bin/unit_tests: $(UNIT_TESTS) $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -r bin/* obj/*