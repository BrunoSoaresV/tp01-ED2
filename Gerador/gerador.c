#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a estrutura do registro
typedef struct {
    int chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} Registro;

int main() {
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
}
