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

// Tabela que contem as palavras reservadas
typedef struct {
    const char* lexema;
    const char* token;
} TabelaReservados;

TabelaReservados tabelaReservados[] = {
    {"program", "simbolo_program"},
    {"var", "var"},
    {"integer", "simbolo_tipo"},
    {"real", "simbolo_tipo"},
    {"begin", "begin"},
    {"end", "end"},
    {"while", "simbolo_while"},
    {"read", "simbolo_read"},
    {"write", "simbolo_write"},
    {"const", "simbolo_const"},
    {"procedure", "simbolo_procedure"},
    {"else", "simbolo_else"},
    {"then", "simbolo_then"},
    {"if", "simbolo_if"},
    {"do", "simbolo_do"},
    {"to", "simbolo_to"},
    {"for", "simbolo_for"},
    {NULL, NULL}  // Marca o fim da tabela
};

const char* operadores[] = {
    ";", ":", "+", "-", "*", "/", "(", ")", "=", ",", ">", "<", "."
};

Token tokens[MAX_TOKENS];
int tokenCount = 0;

void addToken(Token* tokens, int* tokenCount, const char* lexema, const char* token, int linha, int status) {
    if (*tokenCount < MAX_TOKENS) {
        strncpy(tokens[*tokenCount].lexema, lexema, sizeof(tokens[*tokenCount].lexema) - 1);
        strncpy(tokens[*tokenCount].token, token, sizeof(tokens[*tokenCount].token) - 1);
        tokens[*tokenCount].linha = linha;
        tokens[*tokenCount].status = status;
        (*tokenCount)++;
    } else {
        printf("Erro: número máximo de tokens excedido.\n");
    }
}

int isDelimiter(char c) {
    return strchr(";:+-*/()=,<>.", c) != NULL;
}

int isBlank(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void automatoNumero(const char* aux, int num_linha) {
    int s = 0;
    int tama_cont = 0;
    char c;
    int i = 0;

    while ((c = aux[i]) != '\0') {
        switch (s) {
            case 0:
                if (c >= '0' && c <= '9') {
                    s = 2;
                    tama_cont++;
                    if (strlen(aux) == 1) {
                        addToken(tokens, &tokenCount, aux, "numero", num_linha, 1);
                        return;
                    }
                } else if (c == '-' || c == '+') {
                    s = 1;
                } else {
                    s = 5;
                }
                break;

            case 1:
                if (c >= '0' && c <= '9') {
                    tama_cont++;
                    s = 2;
                } else {
                    s = 4;
                }
                break;

            case 2:
                if (i >= (int)strlen(aux) - 1) {
                    if (c >= '0' && c <= '9') {
                        addToken(tokens, &tokenCount, aux, "numero", num_linha, 1);
                    } else {
                        addToken(tokens, &tokenCount, aux, "Numero mal formado", num_linha, 0);
                    }
                    return;
                } else if (c == '.') {
                    s = 3;
                } else if (c >= '0' && c <= '9') {
                    tama_cont++;
                } else {
                    s = 4;
                }
                break;

            case 3:
                if (i >= (int)strlen(aux) - 1) {
                    if (c >= '0' && c <= '9') {
                        addToken(tokens, &tokenCount, aux, "numero", num_linha, 1);
                    } else {
                        addToken(tokens, &tokenCount, aux, "Numero mal formado", num_linha, 0);
                    }
                    return;
                } else if (c >= '0' && c <= '9') {
                    tama_cont++;
                } else {
                    s = 4;
                }
                break;

            case 4:
                addToken(tokens, &tokenCount, aux, "Numero mal formado", num_linha, 0);
                return;

            case 5:
                return;
        }
        i++;
    }
}

void automatoOperadores(char c, const char* linha, int num_linha, int posicao) {
    int s = 0;
    int flag = 1;
    int i = posicao;
    char c_aux;

    while (flag) {
        c_aux = linha[i];

        switch (s) {
            case 0:
                if (c_aux == ':') {
                    s = 1;
                } else if (c_aux == '<') {
                    s = 4;
                } else if (c_aux == '=') {
                    if (linha[i - 1] != ':') {
                        s = 8;
                    } else {
                        s = 19;
                    }
                } else if (c_aux == '>') {
                    if (linha[i - 1] != '<') {
                        s = 9;
                    } else {
                        s = 19;
                    }
                } else if (c_aux == '/') {
                    s = 12;
                } else if (c_aux == ',') {
                    s = 13;
                } else if (c_aux == '*') {
                    s = 14;
                } else if (c_aux == '+') {
                    s = 15;
                } else if (c_aux == '.') {
                    s = 16;
                } else if (c_aux == ';') {
                    s = 17;
                } else if (c_aux == '-') {
                    s = 18;
                }
                break;

            case 1:
                if (c_aux == '=') {
                    s = 2;
                } else {
                    s = 3;
                }
                break;

            case 2:
                addToken(tokens, &tokenCount, ":=", "simbolo_atribuição", num_linha, 1);
                flag = 0;
                break;

            case 3:
                addToken(tokens, &tokenCount, ":", "simbolo_dp", num_linha, 1);
                flag = 0;
                break;

            case 4:
                if (c_aux == '=') {
                    s = 5;
                } else if (c_aux == '>') {
                    s = 6;
                } else {
                    s = 7;
                }
                break;

            case 5:
                addToken(tokens, &tokenCount, "<=", "simbolo_menor_igual", num_linha, 1);
                flag = 0;
                break;

            case 6:
                addToken(tokens, &tokenCount, "<>", "simbolo_dif", num_linha, 1);
                flag = 0;
                break;

            case 7:
                addToken(tokens, &tokenCount, "<", "simbolo_menor", num_linha, 1);
                flag = 0;
                break;

            case 8:
                addToken(tokens, &tokenCount, "=", "simbolo_igual", num_linha, 1);
                flag = 0;
                break;

            case 9:
                if (c_aux == '=') {
                    s = 10;
                } else {
                    s = 11;
                }
                break;

            case 10:
                addToken(tokens, &tokenCount, ">=", "simbolo_maior_igual", num_linha, 1);
                flag = 0;
                break;

            case 11:
                addToken(tokens, &tokenCount, ">", "simbolo_maior", num_linha, 1);
                flag = 0;
                break;

            case 12:
                addToken(tokens, &tokenCount, "/", "simbolo_dividir", num_linha, 1);
                flag = 0;
                break;

             case 13:
                flag = 0;
                break;

            case 14:
                addToken(tokens, &tokenCount, "*", "simbolo_mult", num_linha, 1);
                flag = 0;
                break;

            case 15:
                addToken(tokens, &tokenCount, "+", "simbolo_mais", num_linha, 1);
                flag = 0;
                break;

            case 16:
                addToken(tokens, &tokenCount, ".", "simbolo_ponto", num_linha, 1);
                flag = 0;
                break;

            case 17:
                addToken(tokens, &tokenCount, ";", "simbolo_ponto_virgula", num_linha, 1);
                flag = 0;
                break;

            case 18:
                addToken(tokens, &tokenCount, "-", "simbolo_menos", num_linha, 1);
                flag = 0;
                break;

            case 19:
                flag = 0;
        }
        i++;
    }
}

void automatoIdentificador(const char* palavra, int num_linha) {
    int s = 0;
    int flag = 1;
    int i = 0;
    char c;

    while (flag) {
        c = palavra[i];
        switch (s) {
            case 0:
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    s = 1;
                } else {
                    s = 3;
                }
                break;

            case 1:
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
                    s = 1;
                } else if (c == '\0') {
                    s = 2;
                } else {
                    s = 3;
                }
                break;

            case 2:
                for (int j = 0; tabelaReservados[j].lexema != NULL; j++) {
                    if (strcmp(palavra, tabelaReservados[j].lexema) == 0) {
                        addToken(tokens, &tokenCount, palavra, tabelaReservados[j].token, num_linha, 1);
                        return;
                    }
                }
                addToken(tokens, &tokenCount, palavra, "ident", num_linha, 1);
                flag = 0;
                break;

            case 3:
                addToken(tokens, &tokenCount, palavra, "<ERRO_LEXICO>", num_linha, 0);
                flag = 0;
                break;
        }
        i++;
    }
}

void imprimeTokens(Token tokens[], int tokenCount) {
    FILE *file = fopen("saida.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de saída\n");
        return;
    }
    for (int i = 0; i < tokenCount; i++) {
        fprintf(file, "%s, %s\n", tokens[i].lexema, tokens[i].token);
    }
    fclose(file);
}

int main() {
    FILE *arquivoEntrada = fopen("entrada.txt", "r");
    if (arquivoEntrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    char linha[100];
    int num_linha = 0;

    while (fgets(linha, sizeof(linha), arquivoEntrada) != NULL) {
        num_linha++;
        int tamanho_linha = strlen(linha);
        char palavra[100];
        int j = 0;
        for (int k = 0; k < tamanho_linha; k++) {
            char c = linha[k];
            if (isDelimiter(c) || isBlank(c)) {
                if (j > 0) {
                    palavra[j] = '\0';
                    if (isdigit(palavra[0]) || (palavra[0] == '+' || palavra[0] == '-') && isdigit(palavra[1])) {
                        automatoNumero(palavra, num_linha);
                    } else {
                        automatoIdentificador(palavra, num_linha);
                    }
                    j = 0;
                }
                if (isDelimiter(c)) {
                    automatoOperadores(c, linha, num_linha, k);
                }
            } else {
                palavra[j++] = c;
            }
        }
    }

    fclose(arquivoEntrada);

    imprimeTokens(tokens, tokenCount);

    return 0;
}
