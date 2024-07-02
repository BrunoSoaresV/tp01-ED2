#include <stdio.h>
#include <stdlib.h>
#include "sequencial.h"
#include "struct.h"

int pesquisaCrescente (tipoindice tab[], int tam, Registro* item, FILE *arq, TipoContador *cont) {
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
            if(quantitens == 0) quantitens = ITENSPAGINA; 
        }
        // lê a página desejada do arquivo
        desloc = (i-1)*ITENSPAGINA*sizeof(Registro);

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Registro), quantitens, arq);
        (*cont).leitura++;
        

        //Pesquisa Sequencial na página lida com pesquisa binária
        int inicio = 0;
        int fim = quantitens - 1;
        while (inicio <= fim) {
            int meio = (inicio + fim) / 2;
            (*cont).compChave++;
            if (pagina[meio].chave == item->chave) {
                *item = pagina[meio];
                return 1;
            }
            else if (pagina[meio].chave < item->chave) {
                inicio = meio + 1;
            }
            else {
                fim = meio - 1;
            }
        }
    }
    return 0;  
}

int pesquisaDecrescente(tipoindice tab[], int tam, Registro* item, FILE *arq, TipoContador *cont) {
    Registro pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;

    // procura pela página onde o item pode se encontrar
    i = tam - 1;  // Start from the last index (since we're searching in decreasing order)
    while (i >= 0 && tab[i].chave >= item->chave) i--;

    // caso a chave desejada seja maior que a 1a chave, o item
    // não existe no arquivo
    if (i == tam-1) return 0;

    else {
        // a ultima página pode não estar completa
        if (i >= 0) quantitens = ITENSPAGINA;
        else {
            fseek(arq, 0, SEEK_END);
            quantitens = (ftell(arq) / sizeof(Registro)) % ITENSPAGINA;
            if (quantitens == 0) quantitens = ITENSPAGINA;
        }
        // lê a página desejada do arquivo
        desloc = (i+1) * ITENSPAGINA * sizeof(Registro);

        fseek(arq, desloc, SEEK_SET);
        fread(&pagina, sizeof(Registro), quantitens, arq);
        (*cont).leitura++;

        // Pesquisa binária na página lida
        int inicio = 0;
        int fim = quantitens - 1;
        while (inicio <= fim) {
            int meio = (inicio + fim) / 2;
            (*cont).compChave++;
            if (pagina[meio].chave == item->chave) {
                *item = pagina[meio];
                return 1;
            } else if (pagina[meio].chave > item->chave) {  // For descending order
                inicio = meio + 1;
            } else {
                fim = meio - 1;
            }
        }
    }
    return 0;
}




void geraTabela(tipoindice tabela[], int *pos, Registro x, FILE *arq, TipoContador *cont) {

    while (fread(&x, sizeof(x), 1, arq) == 1) {
        (*cont).leitura++;
        tabela[*pos].chave = x.chave;
        (*pos)++;
        fseek(arq, (ITENSPAGINA-1)* sizeof(x), 1);
    } 
}