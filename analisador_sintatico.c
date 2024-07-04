#include "analisador_sintatico.h"

void erro_parenteses(){
    if(aux_parenteses != 0){
        
        addSintToken("faltou fechar alguns parenteses", "(expressao)", 0, 0);
        aux_parenteses = 0;
    }
}

void addSintToken(char *msgErro, char *cmdEsperado, int status, int outraLinha){
    
    if (linha_analisada < MAX_TOKENS) {
        if(outraLinha > 0){
            strncpy(comandos[termo_Analisado].msgErro, msgErro, sizeof(comandos[termo_Analisado].msgErro) - 1);
            strncpy(comandos[termo_Analisado].cmdEsperado, cmdEsperado, sizeof(comandos[termo_Analisado].cmdEsperado) - 1);
            comandos[termo_Analisado].linha = linha_analisada;
            comandos[termo_Analisado].status = status;
            linha_analisada++;
            termo_Analisado++;
        }else{
            strncpy(comandos[termo_Analisado].msgErro, msgErro, sizeof(comandos[termo_Analisado].msgErro) - 1);
            strncpy(comandos[termo_Analisado].cmdEsperado, cmdEsperado, sizeof(comandos[termo_Analisado].cmdEsperado) - 1);
            comandos[linha_analisada].linha = linha_analisada;
            comandos[termo_Analisado].status = status;
            termo_Analisado++;
        }
    } else {
        printf("Erro: numero maximo de tokens excedido.\n");
    }
}

int var_c(int *cont){
    (*cont)++;
    int chave = 0;
    while(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
        chave = 1;
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_atribuicao") == 0){
            (*cont)++;
            if(strcmp(tokens[*cont].token, "numero") == 0){
                (*cont)++;
                if(strcmp(tokens[*cont].token, "simbolo_virgula") == 0){
                    chave = 0;
                    (*cont)++;
                    continue;
                }
            }else{
                addSintToken("Faltou numero atribuido a constante", "CONST ident := numero;", 0, 1);
                (*cont)++;
                return ERRO;//erro
            }
        }else{
            addSintToken("Faltou simbolo de atribuicao", "CONST ident := numero;", 0, 1);
            (*cont)++;
            (cont)++;
            return ERRO;//erro
        }
    }


    if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0){
        if(chave){
            addSintToken("", "CONST ident := numero;", 1, 1);
            return SUCESSO;
        }else{
            addSintToken("Faltou ident", "CONST ident := numero;", 0, 1);
        }
    }else{
        addSintToken("Faltou ponto e virgula", "CONST ident := numero;", 0, 1);
        return ERRO;//erro
    }
}

int var_v(int *cont){
    (*cont)++;
    int chave = 0;
    while(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
        (*cont)++;
        chave = 1;
        if(strcmp(tokens[*cont].token, "simbolo_virgula") == 0){
            chave = 0;
            (*cont)++;
            continue;
        }
    }
    if((strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0)){ 
        if(chave){
            addSintToken("", "VAR ident;", 1, 1);
            // (*cont)++;
            return SUCESSO;
        }else{
            addSintToken("Faltou ident", "VAR ident;", 0, 1);
            (*cont)++;
        }
    }else{
        addSintToken("Faltou ponto e virgula", "VAR ident;", 0, 1);
        return ERRO;
    }
}

void var_p(int *cont){
    (*cont)++;
    int chave = 0;

    if(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
        (*cont)++;
        chave = 1;
    }

    if(chave && (strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0)){
        addSintToken("", "PROCEDURE ident", 1, 1);
        statusProcedimento = 1;
        (*cont)++;
        bloco(cont);
        return SUCESSO;
    }else{
        addSintToken("Faltou ponto e virgula", "PROCEDURE ident;", 0, 1);
        return ERRO;//erro
    }
}

void declaracao(int *i){
    int chave = 0;
    if(strcmp(tokens[*i].token, "CONST") == 0){//nao sei pq mas ta chamando essa porra duas vezes
        var_c(i);
        (*i)++;
    }
    if(strcmp(tokens[*i].token, "VAR") == 0){
        chave = var_v(i);

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
        if(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
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
    
    if(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
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
        addSintToken("erro de sinal sem numero", "(+)/(-)numero", 0, 0);
    }else{
        expressao_recursiva(cont);
    }

    if(strcmp(tokens[*cont].token, "simbolo_fecha_par") == 0){
        if(aux_parenteses > 0){
            aux_parenteses--;
            expressao(cont);
        }else{
            addSintToken("fechou mais parenteses do que existe", "(expressao)", 0, 0);
        }
    }


}

int condicao(int *cont){
    int chave = 0;
    expressao(cont);
    int retorno = 0;
    
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
    }else if(strcmp(tokens[(*cont)].token, "simbolo_inexistente") == 0){
        chave = 1;
    }

    if(chave == 1){
        expressao(cont);
    }
    
    erro_parenteses();
    return SUCESSO;

}

int cmd(int *cont, int *chave){
    //(*cont)++;

    //ident := expressão
    if(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
        (*cont)++;
        if(strcmp(tokens[*cont].token, "simbolo_atribuicao") == 0){
            expressao(cont);
            erro_parenteses();
            if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0){   
                addSintToken("", "ident := expressao;", 1, 1);                
                (*cont)++;
            }else{
                addSintToken("faltou ponto e virgula", "ident := expressao;",  0, 1); 
            }
            // (*cont)++;
        }else{
            addSintToken("faltou simbolo de atribuicao", "ident := expressao;",  0, 1);
            (*cont++);
        }
    }else if(strcmp(tokens[*cont].token, "simbolo_CALL") == 0){
        (*cont)++;
        if(strcmp(tokens[*cont].token, "ident") == 0 || strcmp(tokens[*cont].token, "<ERRO_LEXICO>") == 0){
            (*cont)++;
            if(strcmp(tokens[*cont].token, "simbolo_ponto_virgula") == 0){     
                addSintToken("", "call ident;", 1, 1);  
                (*cont)++;
            }else{
                addSintToken("faltou ponto e virgula", "CALL ident;", 0, 1); 
                (*cont)++;
            }
        }else{
            addSintToken("faltou identificador", "CALL ident;", 0, 1);
        }
    }else if(strcmp(tokens[*cont].token, "IF") == 0){
        condicao(cont);

        if(strcmp(tokens[*cont].token, "THEN") == 0){
            addSintToken("", "IF condicao THEN", 1, 1);
            (*cont)++;
            
            if(strcmp(tokens[*cont].token, "BEGIN") == 0){ 
            addSintToken("", "BEGIN", 1, 1);
                (*cont)++;
                (*chave)++;
                
                cmd(cont, chave);
            }else{
                addSintToken("faltou BEGIN", "BEGIN", 0, 1);
                (*cont)++;
            }
        }else{
            addSintToken("faltou THEN", "IF condicao THEN", 0, 1);
            (*cont)++;
        }
    }else if(strcmp(tokens[*cont].token, "WHILE") == 0){
        condicao(cont);

        if(strcmp(tokens[*cont].token, "DO") == 0){
            addSintToken("", "WHILE condicao DO", 1, 1);
            (*cont)++;
            
            if(strcmp(tokens[*cont].token, "BEGIN") == 0){ 
            addSintToken("", "BEGIN", 1, 1);
                (*cont)++;
                (*chave)++;
                cmd(cont, chave);
            }else{
                addSintToken("faltou BEGIN", "BEGIN", 0, 1);
                (*chave)++;
                (*cont)++;
            }
        }else{
                addSintToken("faltou DO", "While condicao DO", 0, 1);
            (*cont)++;
        }
    }else if(strcmp(tokens[*cont].token, "<ERRO_COMENT>") == 0){
        addSintToken("faltou fechar comentario", "{}",  0, 1);
        (*cont)++;
    }else if(strcmp(tokens[*cont].token, "COMENTARIO") == 0){
        addSintToken("faltou fechar comentario", "{}",  1, 1);
        (*cont)++;
    }
    else if(strcmp(tokens[*cont].token, "END") == 0 && *chave > 1){
        addSintToken("", "END", 1, 1);
        (*chave)--;
        (*cont)++;
        return SUCESSO;
    }else if(strcmp(tokens[*cont].token, "END") == 0 && *chave == 1){
        addSintToken("", "END",1, 1);
        (*chave)--;
        return SUCESSO;
    }else{
        (*cont)++;
    }
}

int bloco(int *num_token){


    declaracao(num_token);
    int chave_recursividade = 0;

    if(strcmp(tokens[*num_token].token, "BEGIN") == 0){
        addSintToken("", "BEGIN", 1, 1);
        (*num_token)++;
        chave_recursividade++;
    }else{
        addSintToken("Faltou BEGIN", "BEGIN", 0, 1);
        chave_recursividade++;
        (*num_token)++;
        //return ERRO;
    }
    while (/*strcmp(tokens[*num_token].token, "END") != 0 &&*/ chave_recursividade != 0){  
        cmd(num_token, &chave_recursividade);
        if(strcmp(tokens[*num_token].token, "simbolo_ponto") == 0){
            addSintToken("Erro Sintatico", "Faltou END", 0, 1);
            //return ERRO;
        }
    }


    if(strcmp(tokens[*num_token].token, "END") == 0 && statusProcedimento == 0){
        (*num_token)++;
        if (strcmp(tokens[*num_token].token, "simbolo_ponto") == 0) {
        addSintToken("", "END.", 1, 1);
            return SUCESSO;
        }else{
            addSintToken("Faltou ponto", "END.", 0, 0);
            return ERRO;  
        }
    }
    

}

int sintatico(int *num_token){
    linha_analisada = 1;
    termo_Analisado = 0;
    statusProcedimento = 0;
    bloco(num_token);
    // declaracao(num_token);
    // int chave_recursividade = 0;

    // if(strcmp(tokens[*num_token].token, "BEGIN") == 0){
    //     addSintToken("", "BEGIN", linha_analisada, 1);
    //     (*num_token)++;
    // }else{
    //     printf("erro sintatico faltou BEGIN\n");
    //     addSintToken("Faltou BEGIN", "BEGIN", linha_analisada, 0);
    //     // chave_recursividade++;
    //     (*num_token)++;
    //     //return ERRO;
    // }
    // while (/*strcmp(tokens[*num_token].token, "END") != 0 && */chave_recursividade != -1){  
    //     cmd(num_token, &chave_recursividade);
    //     if(strcmp(tokens[*num_token].token, "simbolo_ponto") == 0){
    //         printf("erro sintatico faltou um END\n");
    //         addSintToken("Erro Sintatico", "Faltou END", linha_analisada, 0);
    //         //return ERRO;
    //     }
    // }


    if(strcmp(tokens[*num_token].token, "END") == 0){
        (*num_token)++;
        if (strcmp(tokens[*num_token].token, "simbolo_ponto") == 0) {
        addSintToken("", "END.",1, 1);
            return SUCESSO;
        }else{
        addSintToken("Faltou ponto", "END.",0, 1);
            return ERRO;  
        }
    }
    

}