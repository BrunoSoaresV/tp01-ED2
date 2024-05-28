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

/*int main() {
    // Abrindo o arquivo binário para escrita
    FILE *arquivo = fopen("registrosCemD.bin", "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Gerando 100 registros aleatórios
    for (int i = 100; i > 0; i++) {
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

        // Escrevendo o registro no arquivo
        fwrite(&registro, sizeof(Registro), 1, arquivo);
    }


    // Fechando o arquivo
    fclose(arquivo);

    printf("Arquivo binário gerado com sucesso.\n");

    return 0;
}*/


void shuffle(int *array, int n) {
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main() {
    // Inicializando o gerador de números aleatórios
    srand(time(NULL));

    // Abrindo o arquivo binário para escrita
    FILE *arquivoBin = fopen("registrosCemO.bin", "wb");
    if (arquivoBin == NULL) {
        printf("Erro ao abrir o arquivo binário.\n");
        return 1;
    }

    // Abrindo o arquivo de texto para escrita
    FILE *arquivoTxt = fopen("registrosCemO.txt", "w");
    if (arquivoTxt == NULL) {
        printf("Erro ao abrir o arquivo de texto.\n");
        fclose(arquivoBin);
        return 1;
    }

    // Gerando um array com chaves de 1 a 100
    int chaves[100];
    for (int i = 0; i < 100; i++) {
        chaves[i] = i + 1;
    }

    // Embaralhando as chaves
    shuffle(chaves, 100);

    // Gerando 100 registros aleatórios com chaves únicas
    for (int i = 0; i < 100; i++) {
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

        // Escrevendo o registro no arquivo de texto
        fprintf(arquivoTxt, "Chave: %d\nDado1: %ld\nDado2: %s\nDado3: %s\n\n", 
                registro.chave, registro.dado1, registro.dado2, registro.dado3);
    }

    // Fechando os arquivos
    fclose(arquivoBin);
    fclose(arquivoTxt);

    printf("Arquivos binário e texto gerados com sucesso.\n");

    return 0;
}