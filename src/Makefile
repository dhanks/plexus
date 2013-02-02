####################################################################
# Plexus


CC	= gcc
BIN	= plexus
CFLAGS	= -O2 -Wall -funroll-loops -Wmissing-prototypes

OBJS	= client.o comm.o log.o main.o net.o time.o vplayer.o


$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS)
	mv $(BIN) ..


%.o:
	$(CC) $(CFLAGS) -Iinclude -c -o $*.o $*.c

depend:
	$(CC) -Iinclude -MM *.c > .depend

clean:
	rm -f ../$(BIN) $(BIN) *.o *~

include .depend
