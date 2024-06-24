all: programa

programa: main.o analisador_lexico.o analisador_arquivo.o analisador_sintatico.o
	gcc -o programa main.o analisador_lexico.o analisador_arquivo.o analisador_sintatico.o

main.o: main.c analisador_lexico.h analisador_arquivo.h analisador_sintatico.h
	gcc -c main.c -o main.o

analisador_lexico.o: analisador_lexico.c analisador_lexico.h
	gcc -c analisador_lexico.c -o analisador_lexico.o

analisador_arquivo.o: analisador_arquivo.c analisador_arquivo.h
	gcc -c analisador_arquivo.c -o analisador_arquivo.o

analisador_sintatico.o: analisador_sintatico.c analisador_sintatico.h
	gcc -c analisador_sintatico.c -o analisador_sintatico.o

debug: CFLAGS += -g
debug: programa-debug

programa-debug: main.o analisador_lexico.o analisador_arquivo.o analisador_sintatico.o
	gdb $(CFLAGS) -o programa-debug main.o analisador_lexico.o analisador_arquivo.o analisador_sintatico.o

run: programa
	./programa

clean:
	rm -f programa programa-debug main.o analisador_lexico.o analisador_arquivo.o analisador_sintatico.o

.PHONY: all run clean debug
