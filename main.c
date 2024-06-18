#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analisador_lexico.h"

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

