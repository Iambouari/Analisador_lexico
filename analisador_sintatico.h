#ifndef ANALISADOR_SINTATICO_H
#define ANALISADOR_SINTATICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador_arquivo.h"
#include "analisador_lexico.h"

#define MAX_TOKENS 1000

#define ERRO -1
#define SUCESSO 1

typedef struct {
    char msgErro[100];
    char cmdEsperado[100];
    int linha;
    int status;
} Comando;

Comando comandos[MAX_TOKENS];
int linha_analisada;
int termo_Analisado;
int statusProcedimento;

int aux_parenteses;

// typedef struct {
//     const char* lexema;
//     const char* token;
// } TabelaReservados;


// TabelaReservados tabelaReservados[];

// const char* operadores[];

// Token tokens[MAX_TOKENS];
// int tokenCount;


int sintatico(int *num_token);

#endif //ANALISADOR_LEXICO_H