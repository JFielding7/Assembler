all *.c *.h:
	gcc *.c -o assembler

run:
	gcc *.c -o assembler
	./assembler

clean:
	rm -f assembler