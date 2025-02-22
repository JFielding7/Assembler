CC = gcc
CFLAGS = -Wall -O3

all *.c *.h:
	$(CC) $(CFLAGS) *.c -o compiler

run:
	$(CC) $(CFLAGS) *.c -o compiler
	./compiler

test:
	$(CC) $(CFLAGS) *.c -o compiler
	./compiler test.ro

mem_check:
	$(CC) $(CFLAGS) *.c -o compiler
	valgrind --leak-check=full ./compiler test.dk

clean:
	rm -f compiler