include config.mk

all: obj/c_json_parser_debug.o

release: obj/c_json_parser.o

unit: bin/unit_tests

test:$(TESTS_BIN)

bench: bin/benchmarking

obj/c_json_parser_debug.o: $(HEADERS) $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $@ -DDEBUG

obj/c_json_parser.o: $(HEADERS) $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $@ $(PRODFLAGS)

bin/unit_tests: $(UNIT_TESTS) obj/c_json_parser_debug.o
	$(CC) $(CFLAGS) $^ -o $@

$(TESTS_BIN): $(TESTS) $(DOBJ)
	$(CC) $(CFLAGS) $< $(DOBJ) -o $@

bin/benchmarking: $(OBJ) benchmarking/benchmarking.c benchmarking/gettime.h
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm bin/*
	rm obj/*