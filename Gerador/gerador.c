#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definindo a estrutura do registro
typedef struct {
    int chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} Registro;

int ordenadoCrescente(int, char);
int ordenadoDecrescente(int, char);
int desordenado(int, char);


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

// Função para gerar arquivo texto
/* Código usado para debug
void gerarArquivoTexto() {
    // Abrindo o arquivo texto para escrita
    FILE *arquivoTexto = fopen("registrosMilD.txt", "w");

    if (arquivoTexto == NULL) {
        printf("Erro ao abrir o arquivo texto.\n");
        exit(1);
    }

    // Inicializando o gerador de números aleatórios
    srand(time(NULL));

    // Gerando 100 registros aleatórios
    for (int i = 1000; i > 0; i--) {
        Registro registro;

        // Definindo valores aleatórios para o registro
        registro.chave = i;
        registro.dado1 = rand();
        
        // Preenchendo os dados 2 e 3 com caracteres aleatórios
        for (int j = 0; j < 999; j++) {
            registro.dado2[j] = 'A' + rand() % 26; // caracteres de 'A' a 'Z'
        }
        registro.dado2[999] = '\0'; // Adicionando o terminador de string
        
        for (int j = 0; j < 4999; j++) {
            registro.dado3[j] = 'a' + rand() % 26; // caracteres de 'a' a 'z'
        }
        registro.dado3[4999] = '\0'; // Adicionando o terminador de string

        // Escrevendo o registro no arquivo texto
        fprintf(arquivoTexto, "Chave: %d\nDado1: %ld\nDado2: %s\nDado3: %s\n\n", 
                registro.chave, registro.dado1, registro.dado2, registro.dado3);
    }

    // Fechando o arquivo texto
    fclose(arquivoTexto);

    printf("Arquivo texto gerado com sucesso.\n");
}*/

int main(){
    char sit;
    int tam;

    printf("Defina a situação:\nC: Crescente\nD: Decrescente\nO: Out of Order\n");
    scanf("%c", &sit);

    printf("Defina o tamanho: ");
    scanf("%d", &tam);
    printf("\n\n");

    switch (sit)
    {
    case 'C':
        ordenadoCrescente(tam, sit);
        break;
    case 'D':
        ordenadoDecrescente(tam, sit);
        break;
    case 'O':
        desordenado(tam, sit);
        break;
    default:
        printf("Operação inválida");
        break;
    }

    return 0;
}

int ordenadoCrescente(int tam, char sit){
    //gerarArquivoTexto();
    char filename[50];
    char *qtdStr;
    qtdStr = retornaQuantidade(tam);

    sprintf(filename, "registros%s%c.bin", qtdStr, sit);


    // Abrindo o arquivo binário para escrita
    FILE *arquivoBinario = fopen(filename, "wb");

    if (arquivoBinario == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    // Inicializando o gerador de números aleatórios
    srand(time(NULL));

    // Gerando tam registros aleatórios
    for (int i = 1; i <= tam; i++) {
        Registro registro;

        // Definindo valores aleatórios para o registro
        registro.chave = i;
        registro.dado1 = rand();
        
        // Preenchendo os dados 2 e 3 com caracteres aleatórios
        for (int j = 0; j < 999; j++) {
            registro.dado2[j] = 'A' + rand() % 26; // caracteres de 'A' a 'Z'
        }
        registro.dado2[999] = '\0'; // Adicionando o terminador de string
        
        for (int j = 0; j < 4999; j++) {
            registro.dado3[j] = 'a' + rand() % 26; // caracteres de 'a' a 'z'
        }
        registro.dado3[4999] = '\0'; // Adicionando o terminador de string

        // Escrevendo o registro no arquivo binário
        fwrite(&registro, sizeof(Registro), 1, arquivoBinario);
    }

    // Fechando o arquivo binário
    fclose(arquivoBinario);

    printf("Arquivo binário gerado com sucesso.\n");

    return 0;
}


int ordenadoDecrescente(int tam, char sit){
    //gerarArquivoTexto();
    char filename[50];
    char *qtdStr;
    qtdStr = retornaQuantidade(tam);

    sprintf(filename, "registros%s%c.bin", qtdStr, sit);


    // Abrindo o arquivo binário para escrita
    FILE *arquivoBinario = fopen(filename, "wb");

    if (arquivoBinario == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    // Inicializando o gerador de números aleatórios
    srand(time(NULL));

    // Gerando tam registros aleatórios
    for (int i = tam; i > 0; i--) {
        Registro registro;

        // Definindo valores aleatórios para o registro
        registro.chave = i;
        registro.dado1 = rand();
        
        // Preenchendo os dados 2 e 3 com caracteres aleatórios
        for (int j = 0; j < 999; j++) {
            registro.dado2[j] = 'A' + rand() % 26; // caracteres de 'A' a 'Z'
        }
        registro.dado2[999] = '\0'; // Adicionando o terminador de string
        
        for (int j = 0; j < 4999; j++) {
            registro.dado3[j] = 'a' + rand() % 26; // caracteres de 'a' a 'z'
        }
        registro.dado3[4999] = '\0'; // Adicionando o terminador de string

        // Escrevendo o registro no arquivo binário
        fwrite(&registro, sizeof(Registro), 1, arquivoBinario);
    }

    // Fechando o arquivo binário
    fclose(arquivoBinario);

    printf("Arquivo binário gerado com sucesso.\n");

    return 0;
}



void shuffle(long int *array, int n) {
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int desordenado(int tam, char sit){
    // Inicializando o gerador de números aleatórios
    srand(time(NULL));

    char filename[50];
    char *qtdStr;
    qtdStr = retornaQuantidade(tam);

    sprintf(filename, "registros%s%c.bin", qtdStr, sit);

    // Abrindo o arquivo binário para escrita
    FILE *arquivoBin = fopen(filename, "wb");
    if (arquivoBin == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    // Gerando um array com chaves de 1 a 100000
    long int chaves[tam];
    for (int i = 0; i < tam; i++) {
        chaves[i] = i + 1;
    }

    // Embaralhando as chaves
    shuffle(chaves, tam);

    // Gerando 100 registros aleatórios com chaves únicas
    for (int i = 0; i < tam; i++) {
        Registro registro;

        // Definindo valores para o registro
        registro.chave = chaves[i];
        registro.dado1 = rand();
        
        // Preenchendo os dados 2 e 3 com caracteres aleatórios
        for (int j = 0; j < 999; j++) {
            registro.dado2[j] = 'A' + rand() % 26; // caracteres de 'A' a 'Z'
        }
        registro.dado2[999] = '\0'; // Adicionando o terminador de string
        
        for (int j = 0; j < 4999; j++) {
            registro.dado3[j] = 'a' + rand() % 26; // caracteres de 'a' a 'z'
        }
        registro.dado3[4999] = '\0'; // Adicionando o terminador de string

        // Escrevendo o registro no arquivo binário
        fwrite(&registro, sizeof(Registro), 1, arquivoBin);

    }

    // Fechando os arquivos
    fclose(arquivoBin);

    return 0;
}
