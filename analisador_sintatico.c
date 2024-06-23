#include "analisador_sintatico.h"

int var_c(int *cont){
    (*cont)++;
    int chave = 0;
    while(strcmp(tokens[*cont].token, "ident") == 0){
        chave = 1;
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_atribuicao") == 0){
            (*cont)++;
            if(strcmp(tokens[*cont].token, "numero") == 0){
                (*cont)++;
            }else{
                printf("erro sintatico 1\n");
                return ERRO;//erro
            }
        }else{
            printf("erro sintatico 2\n");
            return ERRO;//erro
        }
    }


    if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0 && chave){
        printf("entrou var_c com sucesso\n");
        return SUCESSO;
    }else{
        printf("erro sintatico 3\n");
        return ERRO;//erro
    }
}

int var_v(int *cont){
    (*cont)++;
    int chave = 0;
    while(strcmp(tokens[*cont].token, "ident") == 0){
        (*cont)++;
        chave = 1;
    }
    if(chave && (strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0)){  
        printf("entrou var_v com sucesso\n");    
        return SUCESSO;
    }else{
        printf("erro sintatico 4\n");
        return ERRO;
    }
}

void var_p(int *cont){
    (*cont)++;
    int chave = 0;

    if(strcmp(tokens[*cont].token, "ident") == 0){
        printf("%s %d", tokens[*cont].token, *cont);
        (*cont)++;
        chave = 1;
    }

    if(chave && (strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0)){
        //sintatico(); isso vai me dar dor de cabeça
        return SUCESSO;
        printf("entrou var_p com sucesso\n");
    }else{
        printf("erro sintatico 5\n");
        return ERRO;//erro
    }
}

void declaracao(int *i){
    if(strcmp(tokens[*i].token, "CONST") == 0){//nao sei pq mas ta chamando essa porra duas vezes
        var_c(i);
        (*i)++;
    }
    if(strcmp(tokens[*i].token, "VAR") == 0){
        var_v(i);
        (*i)++;
    }
    if(strcmp(tokens[*i].token, "PROCEDURE") == 0){
        var_p(i);
        (*i)++;
    }

}

int expressao(int *cont){
    (*cont)++;
}

int condicao(int *cont){
    int chave = 0;
    expressao(cont);
    (*cont)++;

    if (strcmp(tokens[(*cont)].token, "simbolo_menor_igual") == 0){
        chave = 1;
        (*cont)++;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_dif") == 0){
        chave = 1;
        (*cont)++;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_menor") == 0){
        chave = 1;
        (*cont)++;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_igual") == 0){
        chave = 1;
        (*cont)++;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_maior_igual") == 0){
        chave = 1;
        (*cont)++;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_maior") == 0){
        chave = 1;
        (*cont)++;
    }

    if(chave == 1){
        printf("expressao condicao expressao ");
        expressao(cont);
    }else{
        printf("expressao ");
    }

    return SUCESSO;


}

int cmd(int *cont){
    
    //(*cont)++;

    //ident := expressão
    if(strcmp(tokens[*cont].token, "ident") == 0){
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_atribuicao") == 0){
            expressao(cont);
            (*cont)++;
            if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0){   
                printf("ident := expressao;\n");
                (*cont)++;
            }else{
                printf("erro sintatico 7\n");
            }
            // (*cont)++;
        }else{
            printf("erro sintatico 6\n");

        }
    }

    //call ident
    if(strcmp(tokens[*cont].token, "simbolo_CALL") == 0){
        (*cont)++;
        if(strcmp(tokens[*cont].token, "ident") == 0){
            printf("call ident\n");
            (*cont)++;
            if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0){   
                printf("ident := expressao;\n");
                (*cont)++;
            }else{
                printf("erro sintatico 9\n");
            }
        }else{
            printf("erro sintatico 8\n");
        }
    }

    //if condicao THEN comando
    if(strcmp(tokens[*cont].token, "IF") == 0){
        printf("%s ", tokens[*cont].token);
        condicao(cont);

        if(strcmp(tokens[*cont].token, "THEN") == 0){
            printf("%s\n", tokens[*cont].token);
            (*cont)++;
            cmd(cont);
        }else{
            printf("erro sintatico 10\n");
        }
    }

    //while condicao DO comando
    if(strcmp(tokens[*cont].token, "WHILE") == 0){
        condicao(cont);

        if(strcmp(tokens[*cont].token, "DO") == 0){
            printf("while condicao DO comando\n");
            (*cont)++;
            cmd(cont);
        }else{
            printf("erro sintatico 11\n");
        }
    }

}

int sintatico(int *num_token){
    
    declaracao(num_token);

    if(strcmp(tokens[*num_token].token, "BEGIN") == 0){
        printf("%s\n", tokens[*num_token].token);
        (*num_token)++;
    }//else{
    //     printf("erro sintatico\n");
    //     return ERRO;
    // }

    cmd(num_token);




    if(strcmp(tokens[*num_token].token, "END") == 0){
        printf("%s\n", tokens[*num_token].token);
        (*num_token)++;
        if (strcmp(tokens[*num_token].token, "simbolo_ponto") == 0) {
            printf("fim do codigo\n");
            return SUCESSO;
        }else{
            printf("erro sintatico\n");
            return ERRO;  
        }
    }
        
    // }else{
    //     printf("erro sintatico\n");
    //     return ERRO;
    // }

}