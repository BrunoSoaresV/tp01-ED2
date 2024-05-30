#ifndef arvoreB_h
#define arvoreB_h
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#define M 2 //Minimo
#define MM (M*2) //Maximo

//typedef long TipoChave;

typedef struct TipoRegistro{
  //  TipoChave chave;
    Registro dados;
}TipoRegistro;

typedef struct TipoPagina* TipoApontador;//Esse TipoApontador serve para indicar que é um apontador para cada página

typedef struct TipoPagina{
    short n;//indica a quantidade de registro (itens) que tem dentro de cada página
    TipoRegistro r[MM];//vetor de duas posições, onde cada posição guarda um registro
    TipoApontador p[MM + 1];//esse vetor contem +1, pois serve para indicar que são os filhos. Onde cada posição do meu vetor guarda uma variável do TipoApontador. 
}TipoPagina;

typedef struct TipoContador{
    long int transfe;
    long int compChave;
    long int compPag;
}TipoContador;

void inicializa ( TipoApontador* );
bool pesquisaB ( TipoRegistro *,TipoApontador,int*,TipoContador*);
void imprime2 ( TipoApontador,int );
void imprime (TipoApontador);
void insereNaPagina ( TipoApontador,TipoRegistro,TipoApontador);
void ins( TipoRegistro, TipoApontador, short*,TipoRegistro *,TipoApontador * );
void insereB( TipoRegistro,TipoApontador *,TipoContador* );
void iniciarTreeB(FILE* , int , TipoContador *);

#endif//arvoreB_h