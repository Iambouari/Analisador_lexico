all: programa

programa: main.o analisador_lexico.o
	gcc -o programa main.o analisador_lexico.o

main.o: main.c analisador_lexico.h
	gcc -c main.c -o main.o

analisador_lexico.o: analisador_lexico.c analisador_lexico.h
	gcc -c analisador_lexico.c -o analisador_lexico.o

run: programa
	./programa

clean:
	rm -f main.o analisador_lexico.o programa

.PHONY: all run clean
