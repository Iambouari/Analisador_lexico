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
                printf("erro sintatico\n");
                return ERRO;//erro
            }
        }else{
            printf("erro sintatico\n");
            return ERRO;//erro
        }
    }


    if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0 && chave){
        printf("entrou var_c com sucesso\n");
        return SUCESSO;
    }else{
        printf("erro sintatico\n");
        return ERRO;//erro
    }
}

void var_v(int *cont){
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
        printf("erro sintatico\n");
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
        sintatico();
        return SUCESSO;
        printf("entrou var_p com sucesso\n");
    }else{
        printf("erro sintatico\n");
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

int expresao(int *cont){
    
}

int cmd(int *cont){
    if(strcmp(tokens[*cont].token, "ident") == 0){
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_atribuicao") == 0){
            expressao(cont);
        }
    }
}

int sintatico(){
    int i = 0;
    declaracao(&i);
    if(strcmp(tokens[i].token, "BEGIN") == 0){
        i++;
    }else{
        printf("erro sintatico\n");
        return ERRO;
    }

    cmd(&i);




    if(strcmp(tokens[i].token, "END") == 0){
        i++;
        if (strcmp(tokens[i].token, "simbolo_ponto") == 0) {
            printf("fim do codigo\n");
            return SUCESSO;
        }else{
            printf("erro sintatico\n");
            return ERRO;  
        }
        
    }else{
        printf("erro sintatico\n");
        return ERRO;
    }

}