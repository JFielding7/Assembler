all main.c tokenizer.c assembler.h:
	gcc main.c tokenizer.c -o assembler

run:
	gcc main.c tokenizer.c -o assembler
	./assembler

clean:
	rm -f assembler