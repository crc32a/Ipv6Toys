BIN=./bin
CF= -O0 -g -ggdb -I.

all: echo_server lookup echo_client tunnel_server http_hello test_ringcharbuff

test_ringcharbuff.o: ringcharbuff.h test_ringcharbuff.c
	cc $(CF) -c test_ringcharbuff.c

regutil.o: regutil.h regutil.c
	cc $(CF) -c regutil.c

http_hello.o: http_hello.c
	cc $(CF) -c http_hello.c

tunnel_server.o: tunnel_server.c
	cc $(CF) -c tunnel_server.c

echo_client.o: echo_client.c
	cc $(CF) -c echo_client.c

lookup.o: lookup.c
	cc $(CF) -c lookup.c

echo_server.o: echo_server.c
	cc $(CF) -c echo_server.c


socktools.o: socktools.c socktools.h
	cc $(CF) -c socktools.c

ringcharbuff.o: ringcharbuff.c ringcharbuff.h
	cc $(CF) -c ringcharbuff.c

echo_server: echo_server.o socktools.o ringcharbuff.o
	cc $(CF) -o $(BIN)/echo_server echo_server.o socktools.o ringcharbuff.o

lookup: lookup.o socktools.o
	cc $(CF) -o $(BIN)/lookup lookup.o socktools.o

echo_client: echo_client.o socktools.o ringcharbuff.o
	cc $(CF) -o $(BIN)/echo_client echo_client.o socktools.o ringcharbuff.o

tunnel_server: tunnel_server.o socktools.o ringcharbuff.o
	cc $(CF) -o $(BIN)/tunnel_server tunnel_server.o socktools.o ringcharbuff.o

http_hello: http_hello.o socktools.o ringcharbuff.o regutil.o
	cc $(CF) -o $(BIN)/http_hello http_hello.o socktools.o ringcharbuff.o regutil.o

test_ringcharbuff: test_ringcharbuff.o ringcharbuff.o
	cc $(CF) -o $(BIN)/test_ringcharbuff test_ringcharbuff.o ringcharbuff.o
clean:
	rm -rf *.o
	rm ./bin/.kdbg*
	rm ./bin/*
