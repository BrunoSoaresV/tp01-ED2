#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "sequencial.h"
#include "struct.h"
#include "binaria.h"
#include "arvoreB.h"
#include "arvoreBestrela.h"
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
void imprimeChaves(FILE* arq, int tamanho);
int operador(int metodo,int quantidade,int situacao,int chave,int flagP, int argc, char *argv[]);

int main(int argc, char *argv[]){

    // variáveis entradas pelo terminal
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int flagP = (argc == 6) ? atoi(argv[5]) : 0;

    operador(metodo, quantidade, situacao, chave, flagP, argc, argv);

    return 0;
}

int operador(int metodo,int quantidade,int situacao,int chave,int flagP, int argc, char *argv[]){
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

    //variaveis para uso do metodo 4 (arvore B*)
    FILE *arvoreBestr;

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
        start = clock(); //inicia contagem de tempo da geração da tabela
        geraTabela(tabela, &pos, x, arq, &construcao); //Gera a tabela de índices
        end = clock(); //encerra contagem de tempó da geração da tabela
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calcula o tempo de geração da tabela em segundos
        arquivoContagens(chave, 1, quantidade, situacao, "sequencial", construcao); //armazena os dados da geração da tabela em um arquivo

        // ativa a função de pesquisa
        switch (situacao)
        {
        case 1: //Arquivo crescente
            start = clock(); //inicia contagem de tempo da busca
            if (pesquisaCrescente (tabela, pos, &x, arq, &busca)){ //realiza a busca sequencial em arquivo ordenado crescente
                printf ("Registro (codigo %d) foi localizado\n",x.chave);

                if(flagP){ //se for opção do usuário, imprime no terminal as chaves do arquivo
                    imprimeChaves(arq, quantidade); 
                }
            }else{
                printf ("Registro de código %d nao foi localizado\n",x.chave);
            }

            end = clock(); //encerra contagem de tempo da busca
            busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calcula o tempo de busca em segundos
            break;
        
        case 2: //arquivo decrescente
            start = clock(); //inicia contagem de tempo da busca
            if (pesquisaDecrescente (tabela, pos, &x, arq, &busca)){ //realiza a busca sequencial em arquivo ordenado decrescente
                printf ("Registro (codigo %d) foi localizado\n",x.chave);
                //se for opção do usuário, imprime no terminal as chaves do arquivo
                if(flagP){
                    imprimeChaves(arq, quantidade);
                }
            }else{
                printf ("Registro de código %d nao foi localizado\n",x.chave);
            }

            end = clock(); //encerra a contagem de tempo da busca
            busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calcula o tempo de busca em segundos
            break;
        }//FIM DO SWITCH DE PESQUISA

        arquivoContagens(chave, 2, quantidade, situacao, "sequencial", busca); // armazena os dados da busca em um arquivo

        free(tabela);
        fclose (arq);
        
        break;
    
    case 2: //ÁRVORE BINÁRIA
        x.chave = chave;

        // gera a árvore binária
        start = clock(); //inicia a contagem de clocks da geração da árvore binária
        geraArquivoBinaria(arq, quantidade, &construcao); //gera a árvore binária de pesquisa
        end = clock(); //encerra a contagem de clocks da geração da árvore binária
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calcula o tempo de construção em segundos

        if((arquivoContagens(chave, 1, quantidade, situacao, "arvBin\0", construcao)) == false){
            printf("Erro ao gerar arquivo de contagens (Construção : Árvore Binária)");
        }

        // abre o arquivo da árvore binária
        if((arvore = fopen("treefile.bin", "rb")) == NULL){
            printf("Erro na abertura do arquivo árvore pela main\n");
            return 0;
        }
        

        // busca o arquivo na árvore
        start = clock(); //inicia a contagem de clocks da busca
        posNaArvore = buscaBinariaI(arvore, 0, no, chave, &dado, &busca); //realiza a busca na árvore de forma iterativa
        end = clock(); //encerra a contagem de clocks da busca
        busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //armazena o tempo da busca em segundos

        if(posNaArvore!=-1) printf("Registro com chave %d encontrado\n\n", dado.chave);

        //abre arquivo de contagens da busca
        if((arquivoContagens(chave, 2, quantidade, situacao, "arvBin\0",  busca)) == false){
            printf("Erro ao gerar arquivo de contagens (Busca : Árvore Binária)");
        }

        //se for opção do usuário, imprime no terminal as chaves do arquivo
        if(posNaArvore != -1 && flagP){
            imprimeChaves(arq, quantidade);
        }

        fclose(arvore);
        fclose(arq);
        break;

    case 3: //ÁRVORE B   
        //abre arquivo para montar árvore B
        arvoreB = fopen("arvoreB.bin", "wb+");
        if (arvoreB == NULL) {
            printf("Erro ao abrir arquivo da árvore B\n");
            fclose(arq); // Feche o arquivo de registros se falhar ao abrir o arquivo da árvore B
            return 1;
        }
    
        // Constroi a árvore B
        start = clock(); //inicio da contagem de clocks da construção da árvore B
        iniciarTreeB(arq, arvoreB, quantidade,&posRaiz, &construcao);
        end = clock(); // fim da contagem de clocks da construção da árvore B
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calculo do tempo de construção em segundos
        fclose(arvoreB);

        // Abra novamente o arquivo da árvore B para leitura
        arvoreB = fopen("arvoreB.bin", "rb");

        start = clock(); //inicio da contagem de clocks da busca da árvore B
        if (arvoreB == NULL) {
            printf("Erro ao abrir arquivo da árvore B para leitura\n");
            fclose(arq); // Feche o arquivo de registros se houver falha
            return 1;
        }
        if (pesquisaB(&regPesquisa, posRaiz, arvoreB, &busca)) {
            printf("\n\nRegistro com chave %d encontrado\n\n",regPesquisa.dados.chave);
        } else {
            printf("Registro com chave %d não encontrado\n", regPesquisa.dados.chave);
        }
        end = clock(); //fim da contagem de clocks da busca da árvore B
        busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calculo do tempo de busca em segundos
        fclose(arvoreB);

        //se for opção do usuário, imprime no terminal as chaves do arquivo
        if(flagP){
            imprimeChaves(arq, quantidade);
        }

        //cria arquivo de contagens da construção
        if((arquivoContagens(chave, 1, quantidade, situacao, "arvB\0",  construcao)) == false){
            printf("Erro ao gerar arquivo de contagens (Construcao : Árvore B)");
        }

        //cria arquivo de contagens da busca
        if((arquivoContagens(chave, 2, quantidade, situacao, "arvB\0",  busca)) == false){
            printf("Erro ao gerar arquivo de contagens (Busca : Árvore B)");
        }

        fclose(arq);
        break;
    
    //ÁRVORE B ESTRELA
    case 4:

        //Abrindo arquivo da arvore B*
        arvoreBestr = fopen("arvoreBEstrela.bin", "wb+");
        if (arvoreBestr == NULL) {
            printf("Erro ao abrir arquivo da arvore B estrela\n");
            fclose(arq); // Feche o arquivo de registros se falhar ao abrir o arquivo da árvore B
            return 1;
        }

        //Constroi a arvore B*
        start = clock(); //início da contagem de clocks da construção da árvore B*
        iniciarTreeBEstrela( arq, arvoreBestr,quantidade, &posRaiz, &construcao); //constrói a árvore B*
        end = clock(); //fim da contagem de clocks da construção da árvore B*
        construcao.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //calculo do tempo de construção em segundos
        fclose(arvoreBestr);

        //Abrindo o arquivo da arvore B* de novo
        arvoreBestr = fopen("arvoreBEstrela.bin", "rb");

        start = clock(); //início da contagem de clocks da busca da árvore B*
        if (arvoreBestr == NULL) {
            printf("Erro ao abrir arquivo da árvore B estrela para leitura\n");
            fclose(arq); // Feche o arquivo de registros se houver falha
            return 1;
        }
        if (pesquisaBEstrela(&regPesquisa, posRaiz, arvoreBestr, &busca)) {
            printf("\n\nRegistro com chave %d encontrado.\n\n",regPesquisa.dados.chave);
        } else {
            printf("Registro com chave %d não encontrado\n", regPesquisa.dados.chave);
        }
        end = clock(); //fim da contagem de clocks da busca da árvore B*
        busca.tempo = ((double) (end - start)) / CLOCKS_PER_SEC; //cálculo do tempo de busca em segundos
        fclose(arvoreBestr);

        //se for opção do usuário, imprime no terminal as chaves do arquivo
        if(flagP){
            imprimeChaves(arq, quantidade);
        }

        //criação dos arquivos de contagem
        if((arquivoContagens(chave, 1, quantidade, situacao, "arvBEstrela\0",  construcao)) == false){
            printf("Erro ao gerar arquivo de contagens (Construcao : Árvore B Estrela)");
        }

        if((arquivoContagens(chave, 2, quantidade, situacao, "arvBEstrela\0",  busca)) == false){
            printf("Erro ao gerar arquivo de contagens (Busca : Árvore B Estrela)");
        }
        fclose(arq);

        break;

    default:
        break;
    }

    return 0;
}//FIM OPERADOR

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

    // verificação adicional da situação
    if(situacao < 1 || situacao > 3){
        printf("Situação Inválida\n");
        return NULL;
    }

    situacaoStr = retornaSituacao(situacao); // retorna uma string equivalente ao número da situação

    quantidadeStr = retornaQuantidade(quantidade); // retorna uma string equivalente à quantidade

    //com base nos parâmetros de entrada, gera o nome do arquivo de registros a ser aberto
    sprintf(filename, "registros/registros%s%s.bin", quantidadeStr, situacaoStr);
    //abre o arquivo
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

    qtdStr = retornaQuantidade(qtd); // retorna uma string equivalente à quantidade
    sitStr = retornaSituacao(sit); // retorna uma string equivalente ao número da situação
    if(!(strcmp(sitStr, ""))) sitStr = "Crescente";
    
    // gera o nome do arquivo de contagens (as pastas devem existir para funcionar)
    sprintf(filename, "contagens/%s/%s/%s_%s", metodo, qtdStr, sitStr, fase == 1 ? "Construcao" : "Busca");

    // se o arquivo já existe, apenas abre
    if(access(filename, F_OK) == 0){
        arq = fopen(filename, "a");
    }else{ // o arquivo não existe. Então cria-se um arquivo com cabeçalho
        arq = fopen(filename, "w");
        if(arq == NULL) return false;
        fprintf(arq, "%-10s | %-10s | %-10s | %-15s | %-12s | %-12s\n", "Chave", "Leituras", "Escritas", "Transf T", "Comparações", "Tempo_Total");
    }
    if(arq == NULL) return false;

    //Imprime os valores no arquivo (num. de leituras, de escritas, leituras + escritas, comparações entre chaves e tempo)
    fprintf(arq, "%-10d | %-10ld | %-10ld | %-15ld | %-12ld | %-12lf\n", chave, cont.leitura, cont.escrita, cont.leitura + cont.escrita, cont.compChave, cont.tempo);
    fclose(arq);
    return true;
}

char *retornaSituacao(int situacao){
    // retorna uma string de acordo com a situação estabelecida nos parâmetros de entrada
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
    // retorna uma string de acordo com a quantidade de registros estabelecida nos parâmetros de entrada
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
    // inicializa o contador com zeros
    (*cont).leitura = 0;
    (*cont).escrita = 0;
    (*cont).compChave = 0;
}

void imprimeChaves(FILE* arq, int tamanho){
    // itera pelo arquivo de origem imprimindo todas as chaves
    Registro dado;
    fseek(arq, SEEK_SET, 0);
    for(int i = 0; i < tamanho; i++){
        fread(&dado, sizeof(dado), 1, arq);
        printf("%d ", dado.chave);
    }
}