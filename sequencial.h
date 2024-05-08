#ifndef SEQUENCIAL_H
#define SEQUENCIAL_H

#include "struct.h"

#define ITENSPAGINA 4

// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int chave;
} tipoindice;

int pesquisa(tipoindice tab[], int tam, Registro* item, FILE *arq, int situacao);
void geraTabela(tipoindice tabela[], int *pos, Registro x, FILE *arq);

#endif