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
bool geraArquivoBinaria(FILE* origem, int quantidade);
bool insere(Registro dado, int qtdItens, FILE* arvore);
int buscaBinaria(FILE* arvore, int posAtual, tNo no, int chave, Registro *dado);

#endif