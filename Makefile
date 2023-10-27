CC=gcc
ODIR=dist
FILENAME=chadtp
CFLAGS=-Wall

all : build_release

run : build_release
	./$(ODIR)/release/$(FILENAME)

run_dev: build_debug
	./$(ODIR)/debug/$(FILENAME)

build_release : 
	mkdir -p $(ODIR)/release
	$(CC) $(CFLAGS) -O3 src/*.c -o $(ODIR)/release/$(FILENAME)

build_debug : 
	mkdir -p $(ODIR)/debug
	$(CC) $(CFLAGS) -g src/*.c -o $(ODIR)/debug/$(FILENAME)


