CCFLAGS = -Wall -pedantic -std=c99

.PHONY: all clean

all: qotdd #jsmn/libjsmn.a ADD THIS LATER

qotdd: qotdd.o #jsmn/libjsmn.a
	$(CC) $(LDFLAGS) -o $@ $^

qotdd.o: qotdd.c
	$(CC) $(CFLAGS) -c $<

jsmn/libjsmn.a:
	make -C jsmn

clean:
	$(RM) qotdd qotdd.o
	make -C jsmn clean
