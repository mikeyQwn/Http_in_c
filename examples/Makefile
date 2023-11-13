CC=gcc
ODIR=dist
CFLAGS=-Wall -pedantic

build_lib : 
	mkdir -p $(ODIR)/lib
	$(CC) $(CFLAGS) -O3 -c ../src/parsing/*.c ../src/http/*.c ../src/*.c 
	mv *.o $(ODIR)/lib

run_echo_server: build_lib build_echo_server
	./$(ODIR)/example_echo_server

build_echo_server:
	gcc -c echo_server/main.c -o $(ODIR)/echo_server.o
	gcc -fPIC -I/home/lib dist/echo_server.o $(ODIR)/lib/*.o -o $(ODIR)/example_echo_server
	
run_get_time_server: build_lib build_get_time_server
	./$(ODIR)/get_time_server

build_get_time_server:
	gcc -c get_time_server/main.c -o $(ODIR)/get_time_server.o
	gcc -fPIC -I/home/lib dist/get_time_server.o $(ODIR)/lib/*.o -o $(ODIR)/get_time_server
	
