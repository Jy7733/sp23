SRCS = static_main.c
LIBSRCS = open.c close.c set.c get.c 
TARG_ST = kvs_static
TARG_DY = kvs_dynamic

CC = gcc
OPTS = -Wall -g
LIBS = -L. -lkvs

OBJS = $(SRCS:.c=.o)
LOBJS = $(LIBSRCS:.c=.o)
LARS = libkvs.a
LIBD = libkvs.so


all: 

#static library
static: $(TARG_ST)

$(TARG_ST) : $(OBJS) $(LARS)
	$(CC) -o $(TARG_ST) $(OBJS) $(LIBS) 

$(LARS) : $(LOBJS)
	ar rcs $(LARS) $(LOBJS)

#dynamic library
dynamic : $(TARG_DY)

$(TARG_DY) : $(OBJS) $(LIBD)
	$(CC) -o $(TARG_DY) $(OBJS) $(LIBS)

$(LIBD) : $(LOBJS)
	$(CC) -shared -o $(LIBD) $(LOBJS) -fPIC

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LOBJS) $(LARS) $(TARG_ST) $(TARG_DY) $(LIBD)
