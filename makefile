CFLAGS = -Wall -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L
#POSIX 2001, 2008

.PHONY: all clean

all: qotdd 

qotdd: qotdd.o
	$(CC) $(LDFLAGS) -o $@ $^

qotdd.o: qotdd.c
	$(CC) $(CFLAGS) -c $<
	
qotdd_server.o: qotdd_server.c qotdd_server.h
	$(CC) $(CFLAGS) -c $<

jsmn/libjsmn.a:
	make -C jsmn

clean:
	$(RM) qotdd qotdd.o
	make -C jsmn clean
