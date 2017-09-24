CFLAGS = -Wall -pedantic -std=c99

.PHONY: all clean

all: qotdd

qotdd: qotdd.o
	$(CC) $(LDFLAGS) -o $@ $^

qotdd.o: qotdd.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) qotdd qotdd.o
