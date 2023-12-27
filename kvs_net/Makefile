LIBSRCS = open.c close.c set.c get.c 
TARG = kvs_static
CC = gcc
OPTS = -Wall -g
LIBS = -L. -lkvs
LOBJS = $(LIBSRCS:.c=.o)
LARS = libkvs.a

all: static kvs_client kvs_server

# static library
static: $(LARS)

$(LARS) : $(LOBJS)
	ar rcs $(LARS) $(LOBJS)

# kvs_client
kvs_client: kvs_client.o $(LARS)
	$(CC) -o kvs_client kvs_client.o $(LIBS)

kvs_client.o: kvs_client.c
	$(CC) $(OPTS) -c kvs_client.c -o kvs_client.o

# kvs_server
kvs_server: kvs_server.o $(LARS)
	$(CC) -o kvs_server kvs_server.o $(LIBS)

kvs_server.o: kvs_server.c
	$(CC) $(OPTS) -c kvs_server.c -o kvs_server.o

# generic rule for object files
%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(LOBJS) $(LARS) kvs_client.o kvs_client kvs_server.o kvs_server
