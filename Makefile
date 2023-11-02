CC=gcc
ODIR=dist
FILENAME=chadtp
CFLAGS=-Wall -pedantic

all : build_release

run : build_release
	./$(ODIR)/release/$(FILENAME)

run_dev: build_debug
	./$(ODIR)/debug/$(FILENAME)

build_release : 
	mkdir -p $(ODIR)/release
	$(CC) $(CFLAGS) -O3 src/parsing/*.c src/*.c -o $(ODIR)/release/$(FILENAME)

build_debug : 
	mkdir -p $(ODIR)/debug
	$(CC) $(CFLAGS) -g src/parsing/*.c src/*.c -o $(ODIR)/debug/$(FILENAME)


