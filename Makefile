all *.c *.h:
	gcc *.c -o compiler

run:
	gcc *.c -o compiler
	./compiler

test:
	gcc *.c -o compiler
	./compiler test.ro

mem_check:
	gcc *.c -o compiler
	valgrind --leak-check=full ./compiler test.ro

clean:
	rm -f compiler