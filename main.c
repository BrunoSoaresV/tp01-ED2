#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "sequencial.h"
#include "struct.h"
#include <unistd.h>

bool EntradaInvalida(int argc, int metodo, int quantidade, int situacao, int chave);
bool ChaveInvalida(const char* chave);

int main(int argc, char *argv[]){

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    printf("Teste da MAIN\n");
    if(EntradaInvalida(argc, metodo, quantidade, situacao, chave) || ChaveInvalida(argv[4])){
        return 0;
    }

    switch (metodo)
    {
    case 1:
        int pos;
        FILE *arq;

        Registro x;
        x.chave = chave;
        tipoindice *tabela;
        tabela = (tipoindice*) malloc(quantidade * sizeof(tipoindice));
        

        // escolhe qual arquivo será aberto
        switch (quantidade)
        {
        case 100:
            if(situacao == 1){
                if ((arq = fopen("registrosCem.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }else if(situacao == 2){
                if ((arq = fopen("registrosCem.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }
            break;
        case 1000:
            if(situacao == 1){
                if ((arq = fopen("registrosMil.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }else if(situacao == 2){
                if ((arq = fopen("registrosMil.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }
            break;
        case 100000:
            if(situacao == 1){
                if ((arq = fopen("registrosCemMil.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }else if(situacao == 2){
                if ((arq = fopen("registrosCemMil.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }
            break;
        case 1000000:
            if(situacao == 1){
                if ((arq = fopen("registrosMilhao.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }else if(situacao == 2){
                if ((arq = fopen("registrosMilhao.bin","rb")) == NULL) {
                    printf("Erro na abertura do arquivo\n"); return 0;
                }
            }
            break;
        default:
            printf("Não foi possível chamar nenhuma abertura de arquivo\n");
            break;
        }

        // gera tabela de índices
        geraTabela(tabela, &pos, x, arq);

        // ativa a função de pesquisa
        if (pesquisa (tabela, pos, &x, arq, situacao))
            printf ("Registro (codigo %d) foi localizado",x.chave);
        else
            printf ("Registro de código %d nao foi localizado",x.chave);

        fclose (arq);
        
        break;
    
    case 2:
        //árvore binária
        break;
    case 3:
        //Árvore B
        break;
    case 4:
        //Árvore B estrela
        break;
    default:
        break;
    }

    return 0;
}


bool EntradaInvalida(int argc, int metodo, int quantidade, int situacao, int chave){
    //Tratamento de erros

    //Qtd de parâmetros no terminal errada
    if(argc > 6 || argc < 5){
        printf("\nFormato de entrada não compatível");
        return true;
    }

    //Método selecionado inválido
    if(metodo < 1 || metodo > 4){
        printf("\nMétodo indefinido ou não existente");
        return true;
    }

    //Tamanho de arquivo inválido
    if(!(quantidade == 100 || quantidade == 1000 || quantidade == 10000 || quantidade == 100000 || quantidade == 1000000)){
        printf("Tamanho de arquivo não definido");
        return true;
    }

    //Situação selecionada inválida
    if(situacao < 1 || situacao > 3){
        printf("\nSituação indefinida");
        return true;
    }

    //Situação inválida para o método sequencial
    if(metodo == 1 && situacao == 3){
        printf("\nSituação inválida para o método selecionado");
        return true;
    }

    
    return false;
}


bool ChaveInvalida(const char *chave){
    //verifica se a chave está vazia
    if(*chave == '\0'){
        printf("\nChave inválida: A chave não pode ser uma string vazia. ");
        return true;
    }
    //verfica se a chave contem apenas números
    while (*chave != '\0'){
        if(!isdigit(*chave)){
            printf("\nChave inválida: A chave deve conter apenas dígitos. ");
            return true;
        }
        chave ++;
    }
    
    return false;
}