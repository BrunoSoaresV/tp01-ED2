#ifndef BINARIA_H
#define BINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct.h"

typedef struct{
    Registro dados;
    int esq, dir;
}tNo;

tNo criaNovoNo(Registro dados);
bool geraArquivoBinaria(FILE* origem, int quantidade, long int *transf, long int *escrita);
bool insere(Registro dado, int qtdItens, FILE* arvore, long int *transf, long int *escrita);
void imprimeArvore(FILE*, int);
int buscaBinaria(FILE* arvore, int posAtual, tNo no, int chave, Registro *dado, long int *leit);

#endif