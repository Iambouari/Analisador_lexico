#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador_lexico.h"
#include "analisador_arquivo.h"
#include "analisador_sintatico.h"

void imprimeTokens(Token tokens[], int tokenCount) {
    FILE *file = fopen("saida trabalho 1.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de saida\n");
        return;
    }
    for (int i = 0; i < tokenCount; i++) {
        if(strcmp(tokens[i].token, "COMENTARIO") != 0 && strcmp(tokens[i].token, "simbolo_virgula") != 0){
            fprintf(file, "%s, %s\n", tokens[i].lexema, tokens[i].token);
        }
    }
    fclose(file);
}

void imprimeTokensT2(Token tokens[], int tokenCount) {
    FILE *file = fopen("saida oficial.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de saida\n");
        return;
    }
    for (int i = 0; i < tokenCount; i++) {
        if (tokens[i].status == 0)        {
            fprintf(file, "erro lexico linha %d: %s\n", tokens[i].linha,tokens[i].lexema);
        }
    }
    for (int i = 0; i < termo_Analisado; i++) {
        if (comandos[i].status == 0)        {
            fprintf(file, "erro sintatico linha %d: %s sugestão: %s\n", comandos[i].linha,comandos[i].msgErro,comandos[i].cmdEsperado);
        }
    }

    fclose(file);
}

int main() {
    char nomeArquivo[256];

    printf("Digite o nome do arquivo: ");
    if (fgets(nomeArquivo, sizeof(nomeArquivo), stdin) != NULL) {
        nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';
    }

    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo: %s\n", nomeArquivo);
        return 1;
    }

    analisarArquivo(arquivo);

    fclose(arquivo);

    imprimeTokens(tokens, tokenCount);
    imprimeTokensT2(tokens, tokenCount);

    return 0;
}

