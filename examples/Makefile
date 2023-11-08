CC=gcc
ODIR=dist
CFLAGS=-Wall -pedantic

build_lib : 
	mkdir -p $(ODIR)/lib
	$(CC) $(CFLAGS) -O3 -c ../src/parsing/*.c ../src/http/*.c ../src/*.c 
	mv *.o $(ODIR)/lib

run echo_server: build_lib build echo_server
	./$(ODIR)/example_echo_server

build echo_server:
	gcc -c echo_server/main.c -o $(ODIR)/echo_server.o
	gcc -fPIC -I/home/lib dist/*.o $(ODIR)/lib/*.o -o $(ODIR)/example_echo_server
	
