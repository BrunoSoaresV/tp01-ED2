#ifndef SEQUENCIAL_H
#define SEQUENCIAL_H

#include "struct.h"

// definição de uma entrada da tabela de índice das páginas
typedef struct {
    int chave;
} tipoindice;

int pesquisaCrescente(tipoindice tab[], int tam, Registro* item, FILE *arq, TipoContador *busca);
int pesquisaDecrescente(tipoindice tab[], int tam, Registro* item, FILE *arq, TipoContador *busca);
void geraTabela(tipoindice tabela[], int *pos, Registro x, FILE *arq, TipoContador *construcao);

#endif