CFLAGS = -Wall -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L

.PHONY: all clean

all: qotdd qotdd.o qotdd_server.o json_server.o json_parse.o jsmn/libjsmn.a

qotdd: qotdd.o qotdd_server.o json_server.o json_parse.o jsmn/libjsmn.a
	$(CC) $(LDFLAGS) -o $@ $^

qotdd.o: qotdd.c qotdd.h
	$(CC) $(CFLAGS) -c $<
	
qotdd_server.o: qotdd_server.c qotdd_server.h
	$(CC) $(CFLAGS) -c $<
	
json_server.o: json_server.c json_server.h
	$(CC) $(CFLAGS) -c $<
	
json_parse.o: json_parse.c json_parse.h
	$(CC) $(CFLAGS) -c $<

jsmn/libjsmn.a:
	make -C jsmn

clean:
	$(RM) qotdd qotdd.o qotdd_server.o json_server.o json_parse.o jsmn/libjsmn.a
	make -C jsmn clean
