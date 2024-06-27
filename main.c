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
#include <string.h>

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
bool arquivoContagens(int chave, int fase, int qtd, int sit, char metodo[], TipoContador cont);
void iniciaContador(TipoContador *cont);
char *retornaSituacao(int situacao);
char *retornaQuantidade(int quantidade);
void imprimeElemento(int pos, Registro dado);

int main(int argc, char *argv[]){

    // variáveis entradas pelo terminal
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int flagP = (argc == 6) ? atoi(argv[5]) : 0;

    // variáveis para controle de transferências e de tempo
    TipoContador construcao, busca;
    iniciaContador(&construcao);
    iniciaContador(&busca);
    clock_t start, end;


    // variáveis para uso do método 2 (árvore binária)
    tNo no; 
    Registro dado;
    int posNaArvore;
    FILE *arvore;
    int pos;
    Registro x;
    
    //variaveis para uso do método 3 (arvore B)
    FILE *arvoreB;
    long posRaiz;
    TipoRegistro regPesquisa;
    regPesquisa.dados.chave = chave;

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
        geraTabela(tabela, &pos, x, arq, &construcao);
        end = clock();
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;
        arquivoContagens(chave, 1, quantidade, situacao, "sequencial", construcao);

        // ativa a função de pesquisa

        switch (situacao)
        {
        case 1:
            start = clock();
            if (pesquisaCrescente (tabela, pos, &x, arq, &busca)){
                printf ("Registro (codigo %d) foi localizado\n",x.chave);
                // imprime elementos no terminal
                if(flagP){
                    imprimeElemento(0, x); // não há nada guardando a posição dele aq
                }
            }else{
                printf ("Registro de código %d nao foi localizado\n",x.chave);
            }
            end = clock();
            busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;
            break;
        
        case 2:
            start = clock();
            if (pesquisaDecrescente (tabela, pos, &x, arq, &busca)){
                printf ("Registro (codigo %d) foi localizado\n",x.chave);
                // imprime elementos no terminal
                if(flagP){
                    imprimeElemento(0, x); // não há nada guardando a posição dele aq
                }
            }else{
                printf ("Registro de código %d nao foi localizado\n",x.chave);
            }
            end = clock();
            busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;
            break;
        }

        arquivoContagens(chave, 2, quantidade, situacao, "sequencial", busca);


        fclose (arq);
        
        break;
    
    // organização e pesquisa por árvore binária
    case 2:
        x.chave = chave;

        // gera a árvore binária
        start = clock();
        geraArquivoBinaria(arq, quantidade, &construcao); 
        end = clock();
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;

        if((arquivoContagens(chave, 1, quantidade, situacao, "arvBin\0", construcao)) == false){
            printf("Erro ao gerar arquivo de contagens (Construção : Árvore Binária)");
        }

        if((arvore = fopen("treefile.bin", "rb")) == NULL){
            printf("Erro na abertura do arquivo árvore pela main\n");
            return 0;
        }
        
        printf("Iniciando ImprimeÁrvore\n");
        imprimeArvore(arvore, quantidade);
        printf("ImprimeArvore concluído\n");

        // busca o arquivo na árvore
        start = clock();
        posNaArvore = buscaBinariaI(arvore, 0, no, chave, &dado, &busca);
        end = clock();
        busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;
        if((arquivoContagens(chave, 2, quantidade, situacao, "arvBin\0",  busca)) == false){
            printf("Erro ao gerar arquivo de contagens (Busca : Árvore Binária)");
        }

        if(posNaArvore != -1 && flagP){
            imprimeElemento(posNaArvore+1, dado);
        }

        fclose(arvore);
        fclose(arq);
        

        //árvore binária
        break;
    case 3:
        arvoreB = fopen("arvoreB.bin", "wb+");
        if (arvoreB == NULL) {
            printf("Erro ao abrir arquivo da árvore B\n");
            fclose(arq); // Feche o arquivo de registros se falhar ao abrir o arquivo da árvore B
            return 1;
        }
    
        // Constroi a árvore B
        start = clock();
        iniciarTreeB(arq, arvoreB, quantidade,&posRaiz, &construcao);
        end = clock();
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;
        fclose(arvoreB);
        fclose(arq);


     // printf("Árvore iniciada\n");
        printf("\nQuantidade: %d\n", quantidade);
        //printf("posRaiz: %ld\n",posRaiz);
        // Abra novamente o arquivo da árvore B para leitura
        arvoreB = fopen("arvoreB.bin", "rb");

        start = clock();
        if (arvoreB == NULL) {
            printf("Erro ao abrir arquivo da árvore B para leitura\n");
            return 1;
        }
        if (pesquisaB(&regPesquisa, posRaiz, arvoreB, &busca)) {
            printf("\n\nRegistro com chave %d\n encontrado: dado1 = %ld\n dado2 = %s\n dado3 = %s\n\n",regPesquisa.dados.chave, regPesquisa.dados.dado1, regPesquisa.dados.dado2, regPesquisa.dados.dado3);
        } else {
            printf("Registro com chave %d não encontrado\n", regPesquisa.dados.chave);
        }
        end = clock();
        busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC;
        fclose(arvoreB);

        if((arquivoContagens(chave, 1, quantidade, situacao, "arvB\0",  construcao)) == false){
            printf("Erro ao gerar arquivo de contagens (Construcao : Árvore B)");
        }

        if((arquivoContagens(chave, 2, quantidade, situacao, "arvB\0",  busca)) == false){
            printf("Erro ao gerar arquivo de contagens (Busca : Árvore B)");
        }

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

    situacaoStr = retornaSituacao(situacao);

    quantidadeStr = retornaQuantidade(quantidade);

    sprintf(filename, "registros/registros%s%s.bin", quantidadeStr, situacaoStr);
    if((arq = fopen(filename, "rb")) == NULL){
        return NULL; 
    }

    return arq;   
}

bool arquivoContagens(int chave, int fase, int qtd, int sit, char metodo[], TipoContador cont){
    FILE* arq;
    char filename[50];
    char *qtdStr;
    char *sitStr;

    qtdStr = retornaQuantidade(qtd);
    sitStr = retornaSituacao(sit);
    if(!(strcmp(sitStr, ""))) sitStr = "Crescente";
    
    sprintf(filename, "contagens/%s/%s/%s_%s", metodo, qtdStr, sitStr, fase == 1 ? "Construcao" : "Busca");

    if(access(filename, F_OK) == 0){
        arq = fopen(filename, "a");
    }else{
        arq = fopen(filename, "w");
        if(arq == NULL) return false;
        fprintf(arq, "%-10s | %-10s | %-10s | %-15s | %-12s | %-12s\n", "Chave", "Leituras", "Escritas", "Transf T", "Comparações", "Tempo_Total");
    }
    if(arq == NULL) return false;

    fprintf(arq, "%-10d | %-10ld | %-10ld | %-15ld | %-12ld | %-12lf\n", chave, cont.leitura, cont.escrita, cont.leitura + cont.escrita, cont.compChave, cont.tempo);
    fclose(arq);
    return true;
}

char *retornaSituacao(int situacao){
    char *sitStr;
    switch (situacao)
    {
    case 1:
        sitStr = "";
        break;
    case 2:
        sitStr = "D";
        break;
    case 3:
        sitStr = "O";
        break;
    default:
        printf("Situação Inválida; Erro no tratamento de entradas\n");
        return NULL;
    }
    return sitStr;
}

char *retornaQuantidade(int quantidade){
    char *quantidadeStr;
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
    return quantidadeStr;
}

void iniciaContador(TipoContador *cont){
    (*cont).leitura = 0;
    (*cont).escrita = 0;
    (*cont).compChave = 0;
}

void imprimeElemento(int pos, Registro dado){
    printf("\nO elemento procurado está na posição %d\n", pos);
    printf("O elemento buscado é:\n");
    printf("Chave: %d\n", dado.chave);
    printf("Dado1: %ld\n", dado.dado1);
    printf("Dado2: %.10s...\n", dado.dado2);
    printf("Dado3: %.10s...\n", dado.dado3);
}