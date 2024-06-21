#include "analisador_arquivo.h"

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
    sintatico();
}


// void analisarLinha(const char* linha, int num_linha) {
//     int i = 0;
//     char aux[100];
//     int auxIndex = 0;

//     while (linha[i] != '\0') {
//         char c = linha[i];


//         if (isBlank(c) || c == '{' || c == '}') {
//             if (auxIndex > 0) {
//                 aux[auxIndex] = '\0';
//                 if (isdigit(aux[0]) || (aux[0] == '+' || aux[0] == '-') && isdigit(aux[1])) {
//                     automatoNumero(aux, num_linha);
//                 } else {
//                     automatoIdentificador(aux, num_linha);

//                 }
//                 auxIndex = 0;
//             }
//             if (c == '{' || c == '}') {
//                 automatoComentario(linha, &i, num_linha);
//             }
//             i++;
//             continue;
//         }

//         if (isDelimiter(c)) {
//             if (auxIndex > 0) {
//                 aux[auxIndex] = '\0';
//                 if (isdigit(aux[0]) || (aux[0] == '+' || aux[0] == '-') && isdigit(aux[1])) {
//                     automatoNumero(aux, num_linha);
//                 } else {
//                    automatoIdentificador(aux, num_linha);
//                 }
//                 auxIndex = 0;
//             }

//             automatoOperadores(c, linha, num_linha, i);
//             i++;
//             continue;
//         }

//         aux[auxIndex++] = c;
//         i++;
//     }


//     if (auxIndex > 0) {
//         aux[auxIndex] = '\0';
//         if (isdigit(aux[0]) || (aux[0] == '+' || aux[0] == '-') && isdigit(aux[1])) {
//             automatoNumero(aux, num_linha);
//         } else {
//             automatoIdentificador(aux, num_linha);
//         }
//     }

//     sintatico();
// }

void analisarArquivo(FILE* arquivo) {
    char linha[256];
    int num_linha = 1;

    while (fgets(linha, sizeof(linha), arquivo)) {
        analisarLinha(linha, num_linha);
        num_linha++;
    }
}