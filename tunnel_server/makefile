BIN=./bin
CF= -O3 -g -ggdb -I.

all: echo_server lookup echo_client tunnel_server http_hello test_ringcharbuff \
    test_regutil swallow_server swallow_client http_get

http_get.o: http_get.c
	cc $(CF) -c http_get.c

test_regutil.o: test_regutil.c
	cc $(CF) -c test_regutil.c

test_ringcharbuff.o: ringcharbuff.h test_ringcharbuff.c
	cc $(CF) -c test_ringcharbuff.c

regutil.o: regutil.h regutil.c regutil.h
	cc $(CF) -c regutil.c

http_hello.o: http_hello.c
	cc $(CF) -c http_hello.c

swallow_client.o: swallow_client.c
	cc $(CF) -c swallow_client.c

swallow_server.o: swallow_server.c
	cc $(CF) -c swallow_server.c

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

swallow_client: swallow_client.o socktools.o ringcharbuff.o
	cc $(CF) -o $(BIN)/swallow_client swallow_client.o socktools.o ringcharbuff.o

tunnel_server: tunnel_server.o socktools.o ringcharbuff.o
	cc $(CF) -o $(BIN)/tunnel_server tunnel_server.o socktools.o ringcharbuff.o

swallow_server: swallow_server.o socktools.o ringcharbuff.o regutil.o
	cc $(CF) -o $(BIN)/swallow_server swallow_server.o socktools.o ringcharbuff.o regutil.o -lpcre

http_hello: http_hello.o socktools.o ringcharbuff.o regutil.o
	cc $(CF) -o $(BIN)/http_hello http_hello.o socktools.o ringcharbuff.o regutil.o -lpcre

test_ringcharbuff: test_ringcharbuff.o ringcharbuff.o
	cc $(CF) -o $(BIN)/test_ringcharbuff test_ringcharbuff.o ringcharbuff.o

http_get: http_get.o socktools.o ringcharbuff.o
	cc $(CF) -o $(BIN)/http_get http_get.o socktools.o ringcharbuff.o

test_regutil: test_regutil.o regutil.o
	cc $(CF) -o $(BIN)/test_regutil test_regutil.o regutil.o -lpcre

clean:
	rm -rf *.o
	rm ./bin/*
	rm ./bin/.kdbg*
