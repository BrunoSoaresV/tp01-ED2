#include <stdio.h>
#include <stdlib.h>
#include "sequencial.h"
#include "struct.h"

int pesquisa (tipoindice tab[], int tam, Registro* item, FILE *arq) {
    Registro pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;

    // procura pela página onde o item pode se encontrar
    i = 0;
    while (i < tam && tab[i].chave <= item->chave) i++;

    // caso a chave desejada seja menor que a 1a chave, o item
    // não existe no arquivo
    if (i == 0) return 0;
    else {
        // a ultima página pode não estar completa
        if (i < tam) quantitens = ITENSPAGINA;
        else {
            fseek (arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(Registro))%ITENSPAGINA;
        }
        // lê a página desejada do arquivo
        desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(Registro);

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Registro), quantitens, arq);
        // pesquisa sequencial na página lida
        for (i=0; i < quantitens; i++){
            if (pagina[i].chave == item->chave) {
                *item = pagina[i]; return 1;
            }
        }
    }
    return 0;  
}

void geraTabela(tipoindice tabela[], int *pos, Registro x, FILE *arq) {
    int cont = 0;
    while (fread(&x, sizeof(x), 1, arq) == 1) {
        cont++;
        if (cont % ITENSPAGINA == 1) {
            tabela[*pos].chave = x.chave;
            tabela[*pos].posicao = (*pos)+1;
            (*pos)++;
        }
    } 
}

