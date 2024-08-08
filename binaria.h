#ifndef BINARIA_H
#define BINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct.h"

// Estrutura básica da árvore
typedef struct{
    Registro dados;
    int esq, dir;
}tNo;

// Função que cria um novo nó
tNo criaNovoNo(Registro dados);

// Função que gera a árvore binária em um arquivo
bool geraArquivoBinaria(FILE* origem, int quantidade, TipoContador*);

// Função que insere registros na árvore
bool insere(Registro dado, int qtdItens, FILE* arvore, TipoContador*);

// Função que realiza uma busca binária iterativa em uma árvore armazenada em um arquivo
int buscaBinariaI(FILE* arvore, int posAtual, tNo no, int chave, Registro *dado, TipoContador *cont);

#endif