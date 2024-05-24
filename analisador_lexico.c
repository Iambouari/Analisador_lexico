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

TabelaReservados tabelaReservados[] = {
    {"CALL","simbolo_CALL"},
    {"PROGRAM", "PROGRAM"},
    {"VAR", "VAR"},
    {"INTEGER", "INTEGER"},
    {"REAL", "REAL"},
    {"BEGIN", "BEGIN"},
    {"END", "END"},
    {"WHILE", "WHILE"},
    {"READ", "READ"},
    {"WRITE", "WRITE"},
    {"CONST", "CONST"},
    {"PROCEDURE", "PROCEDURE"},
    {"ELSE", "ELSE"},
    {"THEN", "THEN"},
    {"IF", "IF"},
    {"DO", "DO"},
    {"TO", "TO"},
    {"FOR", "FOR"},
    {"USES", "USES"},
    {"CRT", "CRT"},
    {"UNTIL", "UNTIL"},
    {"REPEAT", "REPEAT"},
    {NULL, NULL}
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
        printf("Erro: numero maximo de tokens excedido.\n");
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
                } else if (c_aux == '(') {
                    s = 19;
                } else if (c_aux == ')') {
                    s = 20;
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
                addToken(tokens, &tokenCount, ":=", "simbolo_atribuicao", num_linha, 1);
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
                addToken(tokens, &tokenCount, "(", "simbolo_par", num_linha, 1);
                flag = 0;
                break;

            case 20:
                addToken(tokens, &tokenCount, ")", "simbolo_par", num_linha, 1);
                flag = 0;
                break;

            case 21:
                flag = 0;
        }
        i++;
    }
}

void automatoComentario(const char* linha, int* pos, int num_linha) {
    int s = 0;
    char c;
    char comentario[256];
    int index = 0;

    while ((c = linha[*pos]) != '\0') {
        comentario[index++] = c;
        switch (s) {
            case 0:
                if (c == '{') {
                    s = 1;
                }
                break;

            case 1:
                if (c == '}') {
                    return;
                }
                break;
        }
        (*pos)++;
    }

    comentario[index] = '\0'; // Termina a string de comentário

    if (s == 1) {
        addToken(tokens, &tokenCount, comentario, "<ERRO_COMENT>", num_linha, 0);
    }
}

void automatoIdentificador(const char* aux, int num_linha) {
    int s = 0;
    int i = 0;
    char c;

    while ((c = aux[i]) != '\0') {
        switch (s) {
            case 0:
                if (isalpha(c)) {
                    s = 1;
                } else {
                    s = 2;
                }
                break;

            case 1:
                if (!(isalnum(c) || c == '_')) {
                    s = 2;
                }
                break;

            case 2:
                addToken(tokens, &tokenCount, aux, "<ERRO_LEXICO>", num_linha, 0);
                return;
        }
        i++;
    }

    if (s == 1) {
        for (int j = 0; tabelaReservados[j].lexema != NULL; j++) {
            if (strcmp(tabelaReservados[j].lexema, aux) == 0) {
                addToken(tokens, &tokenCount, aux, tabelaReservados[j].token, num_linha, 1);
                return;
            }
        }
        addToken(tokens, &tokenCount, aux, "ident", num_linha, 1);
    } else {
        addToken(tokens, &tokenCount, aux, "<ERRO_LEXICO>", num_linha, 0);
    }
}

void analisarLinha(const char* linha, int num_linha) {
    int i = 0;
    char aux[100];
    int auxIndex = 0;

    while (linha[i] != '\0') {
        char c = linha[i];

        if (isBlank(c) || c == '{' || c == '}') {
            if (auxIndex > 0) {
                aux[auxIndex] = '\0';
                if (isdigit(aux[0]) || (aux[0] == '+' || aux[0] == '-') && isdigit(aux[1])) {
                    automatoNumero(aux, num_linha);
                } else {
                    automatoIdentificador(aux, num_linha);
                }
                auxIndex = 0;
            }
            if (c == '{' || c == '}') {
                automatoComentario(linha, &i, num_linha);
            }
            i++;
            continue;
        }

        if (isDelimiter(c)) {
            if (auxIndex > 0) {
                aux[auxIndex] = '\0';
                if (isdigit(aux[0]) || (aux[0] == '+' || aux[0] == '-') && isdigit(aux[1])) {
                    automatoNumero(aux, num_linha);
                } else {
                    automatoIdentificador(aux, num_linha);
                }
                auxIndex = 0;
            }

            automatoOperadores(c, linha, num_linha, i);
            i++;
            continue;
        }

        aux[auxIndex++] = c;
        i++;
    }

    if (auxIndex > 0) {
        aux[auxIndex] = '\0';
        if (isdigit(aux[0]) || (aux[0] == '+' || aux[0] == '-') && isdigit(aux[1])) {
            automatoNumero(aux, num_linha);
        } else {
            automatoIdentificador(aux, num_linha);
        }
    }
}

void analisarArquivo(FILE* arquivo) {
    char linha[256];
    int num_linha = 1;

    while (fgets(linha, sizeof(linha), arquivo)) {
        analisarLinha(linha, num_linha);
        num_linha++;
    }
}

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
