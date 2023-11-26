ST_SRC = static_main.c
DY_SRC = dynamic_main.c

LIBSRCS = open.c close.c set.c get.c 
TARG_ST = kvs_static
TARG_DY = kvs_dynamic

CC = gcc
OPTS = -Wall -g
LIBS = -L. -lkvs
LIBD= -L. -lkvs -ldl

ST_OBJS = $(ST_SRC:.c=.o)
DY_OBJS = $(DY_SRC:.c=.o)

LOBJS = $(LIBSRCS:.c=.o)
LARS = libkvs.a
LSO = libkvs.so


all: 

#static library
static: $(TARG_ST)

$(TARG_ST) : $(ST_OBJS) $(LARS)
	$(CC) -o $(TARG_ST) $(ST_OBJS) $(LIBS) 

$(LARS) : $(LOBJS)
	ar rcs $(LARS) $(LOBJS)

#dynamic library
dynamic : $(TARG_DY)

$(TARG_DY) : $(DY_OBJS) $(LSO)
	$(CC) -o $(TARG_DY) $(DY_OBJS) $(LIBD)

$(LSO) : $(LOBJS)
	$(CC) -shared -fPIC -o $(LSO) $(LIBSRCS) -ldl

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(ST_OBJS) $(DY_OBJS) $(LOBJS) $(LARS) $(TARG_ST) $(TARG_DY) $(LSO)
