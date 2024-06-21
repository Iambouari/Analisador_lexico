#ifndef ANALISADOR_ARQUIVO_H
#define ANALISADOR_ARQUIVO_H

#include "analisador_lexico.h"
#include "analisador_sintatico.h"

void analisarArquivo(FILE* arquivo);
void imprimeTokens(Token tokens[], int tokenCount);

#endif