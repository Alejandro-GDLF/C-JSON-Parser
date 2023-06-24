CC=gcc
CFLAGS=-g -Wall -Wextra
PRODFLAGS= -Wl,--strip-all

HEADERS=headers/c_json_parser.h headers/c_json_parser_types.h
SRC=src/c_json_parser.c
OBJ=obj/c_json_parser.o
DOBJ=obj/c_json_parser_debug.o

UNIT_TESTS=$(wildcard tests/unit_tests/*.c)
TESTS=$(wildcard tests/tests/*.c)
TESTS_BIN=$(patsubst tests/tests/%.c, bin/%, $(TESTS))

SUBMITNAME=c_json_parser

all: obj/c_json_parser_debug.o

obj/c_json_parser_debug.o: $(HEADERS) $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $@


unit: bin/unit_tests

bin/unit_tests: $(UNIT_TESTS) $(DOBJ)
	$(CC) $(CFLAGS) $^ -o $@


test:$(TESTS_BIN)

$(TESTS_BIN): $(TESTS) $(DOBJ)
	$(CC) $(CFLAGS) $< $(DOBJ) -o $@

release: obj/c_json_parser.o

obj/c_json_parser.o: $(HEADERS) $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $@ $(PRODFLAGS)

clean:
	rm bin/*
	rm obj/*