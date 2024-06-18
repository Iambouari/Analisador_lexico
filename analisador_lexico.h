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


void analisarArquivo(FILE* arquivo);
void imprimeTokens(Token tokens[], int tokenCount);

#endif //ANALISADOR_LEXICO_H