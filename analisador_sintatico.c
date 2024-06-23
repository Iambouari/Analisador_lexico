#include "analisador_sintatico.h"

void erro_parenteses(){
    if(aux_parenteses != 0){
        printf("\n\nfaltou fechar alguns parenteses\n\n");
    }
}

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

int expressao_recursiva(int *cont){

    if (strcmp(tokens[*cont].token, "simbolo_menos") == 0 || 
    strcmp(tokens[*cont].token, "simbolo_mais") == 0 ||
    strcmp(tokens[*cont].token, "simbolo_mult") == 0 ||
    strcmp(tokens[*cont].token, "simbolo_dividir") == 0){

        (*cont)++;
        if(strcmp(tokens[*cont].token, "ident") == 0){
            (*cont)++;
        }

        if(strcmp(tokens[*cont].token, "numero") == 0){
            (*cont)++;
            if(strcmp(tokens[*cont].token, "simbolo_ponto") == 0){
                (*cont)++;
                if(strcmp(tokens[*cont].token, "numero") == 0){
                    (*cont)++;
                }else{
                    printf("numero real mal formatado\n");
                }
            }
        }
        expressao_recursiva(cont);
    }
    if(strcmp(tokens[*cont].token, "simbolo_abre_par") == 0){
        aux_parenteses++;
        expressao(cont);
    }

}

int expressao(int *cont){
    (*cont)++;
    int aux_sinal = 0;

    if (strcmp(tokens[*cont].token, "simbolo_menos") == 0 ||
    strcmp(tokens[*cont].token, "simbolo_mais") == 0 ){
        aux_sinal = 1;
        (*cont)++;
    }
    
    if(strcmp(tokens[*cont].token, "ident") == 0){
        aux_sinal = 0;
        (*cont)++;
    }

    

    if(strcmp(tokens[*cont].token, "numero") == 0){
        aux_sinal = 0;
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_ponto") == 0){
            (*cont)++;
            if(strcmp(tokens[*cont].token, "numero") == 0){
                (*cont)++;
            }else{
                printf("numero real mal formatado\n");
            }
        }
    }

    if(strcmp(tokens[*cont].token, "simbolo_abre_par") == 0){
        aux_parenteses++;
        aux_sinal = 0;
        expressao(cont);
    }

    if(aux_sinal == 1){
        printf("erro\n");
    }else{
        expressao_recursiva(cont);
    }

    if(strcmp(tokens[*cont].token, "simbolo_fecha_par") == 0){
        if(aux_parenteses > 0){
            aux_parenteses--;
            expressao(cont);
        }else{
            printf("fechou mais parenteses do que existe");
        }
    }


}

int condicao(int *cont){
    int chave = 0;
    expressao(cont);
    
    if (strcmp(tokens[(*cont)].token, "simbolo_menor_igual") == 0){
        chave = 1;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_dif") == 0){
        chave = 1;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_menor") == 0){
        chave = 1;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_igual") == 0){
        chave = 1;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_maior_igual") == 0){
        chave = 1;
    }else if(strcmp(tokens[(*cont)].token, "simbolo_maior") == 0){
        chave = 1;
    }

    if(chave == 1){
        printf("expressao condicao expressao ");
        expressao(cont);
    }else{
        printf("expressao ");
    }
    
    erro_parenteses();
    return SUCESSO;


}

int cmd(int *cont, int *chave){
    
    //(*cont)++;

    //ident := expressão
    if(strcmp(tokens[*cont].token, "ident") == 0){
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_atribuicao") == 0){
            expressao(cont);
            erro_parenteses();
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
            (*cont)++;
            if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0){       
                printf("call ident\n");
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
            if(strcmp(tokens[*cont].token, "BEGIN") == 0){ 
                printf("%s\n", tokens[*cont].token);
                (*cont)++;
                (*chave)++;
                cmd(cont, chave);
            }else{
                printf("erro sintatico 12\n");
            }
        }else{
            printf("erro sintatico 10\n");
        }
    }

    //while condicao DO comando
    if(strcmp(tokens[*cont].token, "WHILE") == 0){
        printf("%s ", tokens[*cont].token);
        condicao(cont);

        if(strcmp(tokens[*cont].token, "DO") == 0){
            printf("%s\n", tokens[*cont].token);
            (*cont)++;
            if(strcmp(tokens[*cont].token, "BEGIN") == 0){ 
                printf("%s\n", tokens[*cont].token);
                (*cont)++;
                (*chave)++;
                cmd(cont, chave);
            }else{
                printf("erro sintatico 13\n");
            }
        }else{
            printf("erro sintatico 11\n");
        }
    }


    if(strcmp(tokens[*cont].token, "END") == 0 && *chave > 0){
        printf("%s\n", tokens[*cont].token);
        (*chave)--;
        (*cont)++;
        return SUCESSO;
    }else if(strcmp(tokens[*cont].token, "END") == 0 && *chave == 0){
        (*chave)--;
        return SUCESSO;
    }
}

int sintatico(int *num_token){
    
    declaracao(num_token);
    int chave_recursividade = 0;

    if(strcmp(tokens[*num_token].token, "BEGIN") == 0){
        printf("%s\n", tokens[*num_token].token);
        (*num_token)++;
    }//else{
    //     printf("erro sintatico\n");
    //     return ERRO;
    // }
    while (/*strcmp(tokens[*num_token].token, "END") != 0 && */chave_recursividade != -1){  
        cmd(num_token, &chave_recursividade);
        if(strcmp(tokens[*num_token].token, "simbolo_ponto") == 0){
            printf("erro sintatico faltou um END\n");
            return ERRO;
        }
    }


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