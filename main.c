#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador_lexico.h"
#include "analisador_arquivo.h"

void imprimeTokens(Token tokens[], int tokenCount) {
    FILE *file = fopen("saida.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de saida\n");
        return;
    }
    for (int i = 0; i < tokenCount; i++) {
        fprintf(file, "%s, %s\n", tokens[i].lexema, tokens[i].token);
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

    return 0;
}

