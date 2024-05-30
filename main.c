#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "sequencial.h"
#include "struct.h"
#include "binaria.h"
#include "arvoreB.h"
#include <unistd.h>
#include <time.h>

/*  SOBRE OS ARQUIVOS 
    Todos os arquivos com os dados de teste foram gerados previamente e preenchidos de maneira aleatória (três arquivos por quantidade)

    O nome dos arquivos binários segue a seguinte lógica:

    "registros/registros<quantidade><situacao>.bin"

    Sendo as situações:
    1° Ordenado crescente <>
    2° Ordenado decrescente <D>
    3° Desordenado <O> (Out of Order)

*/

bool EntradaInvalida(int argc, int metodo, int quantidade, int situacao, int chave);
bool ChaveInvalida(const char* chave);
FILE* abreArquivo(int, int);

int main(int argc, char *argv[]){

    // variáveis entradas pelo terminal
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);

    // variáveis para controle de transferências
    long int leit = 0, escrita = 0, transf;

    // variáveis para controle de tempo
    clock_t start, end;
    double tempo_criacao, tempo_busca;

    // variáveis para uso do método 2 (árvore binária)
    tNo no; 
    Registro dado;
    int posNaArvore;
    FILE *arvore;
    int pos;
    Registro x;
    
    // Verifica se as entradas são válidas
    if(EntradaInvalida(argc, metodo, quantidade, situacao, chave) || ChaveInvalida(argv[4])){
        return 0;
    }
    
    // define e abre o arquivo escolhido
    FILE *arq;
    arq = abreArquivo(situacao, quantidade);
    if(arq == NULL) return 0;


    // escolhe o método
    switch (metodo)
    {
    case 1:
        x.chave = chave;
        tipoindice *tabela;
        tabela = (tipoindice*) malloc(quantidade * sizeof(tipoindice));
        
        // gera tabela de índices
        start = clock();
        geraTabela(tabela, &pos, x, arq);
        end = clock();
        tempo_criacao = ((double) (end - start)) / CLOCKS_PER_SEC;

        // ativa a função de pesquisa
        start = clock();
        if (pesquisa (tabela, pos, &x, arq, situacao))
            printf ("Registro (codigo %d) foi localizado",x.chave);
        else
            printf ("Registro de código %d nao foi localizado",x.chave);
        end = clock();
        tempo_busca = ((double) (end - start)) / CLOCKS_PER_SEC;


        fclose (arq);
        
        break;
    
    // organização e pesquisa por árvore binária
    case 2:
        x.chave = chave;

        // gera a árvore binária
        start = clock();
        geraArquivoBinaria(arq, quantidade, &leit, &escrita);
        end = clock();
        tempo_criacao = ((double) (end - start)) / CLOCKS_PER_SEC;

        if((arvore = fopen("treefile.bin", "rb")) == NULL){
            printf("Erro na abertura do arquivo árvore pela main\n");
            return 0;
        }
        
        imprimeArvore(arvore, quantidade);

        // busca o arquivo na árvore
        start = clock();
        posNaArvore = buscaBinaria(arvore, 0, no, chave, &dado, &leit);
        end = clock();
        tempo_busca = ((double) (end - start)) / CLOCKS_PER_SEC;

        if(posNaArvore != -1){
            printf("\nO elemento procurado está na posição %d\n", posNaArvore+1);
            printf("O elemento buscado é:\n");
            printf("Chave: %d\n", dado.chave);
            printf("Dado1: %ld\n", dado.dado1);
            printf("Dado2: %.10s...\n", dado.dado2);
            printf("Dado3: %.10s...\n", dado.dado3);
        }

        transf = leit + escrita;
        printf("\nLeitu | Escr | Transf\n");
        printf("%ld \t%ld \t%ld\n", leit, escrita, transf);
        printf("\nTEMPO\nCriação da árvore binária: %lf\nBusca do registro: %lf\n\n", tempo_criacao, tempo_busca);
        

        //árvore binária
        break;
    case 3:
        // Chamar a iniciarTreeB
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

FILE* abreArquivo(int situacao, int quantidade){
    FILE* arq;
    char filename[50];
    char* situacaoStr;
    char* quantidadeStr;

    if(situacao < 1 || situacao > 3){
        printf("Situação Inválida\n");
        return NULL;
    }

    switch (situacao)
    {
    case 1:
        situacaoStr = "";
        break;
    case 2:
        situacaoStr = "D";
        break;
    case 3:
        situacaoStr = "O";
        break;
    default:
        printf("Situação Inválida; Erro no tratamento de entradas\n");
        return NULL;
    }

    switch (quantidade)
    {
    case 100:
        quantidadeStr = "Cem";
        break;
    case 1000:
        quantidadeStr = "Mil";
        break;
    case 10000:
        quantidadeStr = "DezMil";
        break;
    case 100000:
        quantidadeStr = "CemMil";
        break;
    case 1000000:
        quantidadeStr = "UmMilhao";
        break;
    
    default:
        printf("Quantidade Inválida; Erro no tratamento de entradas\n");
        return NULL;
        break;
    }

    sprintf(filename, "registros/registros%s%s.bin", quantidadeStr, situacaoStr);
    if((arq = fopen(filename, "rb")) == NULL){
        return NULL; 
    }

    return arq;   
}
