#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1000

typedef struct {
    char lex;
    char lexema[100];
    char token[100];
    int linha;
    int status;
} Token;

typedef struct {
    const char* lexema;
    const char* token;
} TabelaReservados;


TabelaReservados tabelaReservados[];

const char* operadores[];

Token tokens[MAX_TOKENS];
int tokenCount;



int isBlank(char c);
int isDelimiter(char c);
void automatoNumero(const char* aux, int num_linha);
void automatoOperadores(char c, const char* linha, int num_linha, int posicao);
void automatoComentario(const char* linha, int* pos, int num_linha);
void automatoIdentificador(const char* aux, int num_linha);

// void analisarArquivo(FILE* arquivo);
// void imprimeTokens(Token tokens[], int tokenCount);

#endif //ANALISADOR_LEXICO_H