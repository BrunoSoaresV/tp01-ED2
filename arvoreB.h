#ifndef arvoreB_h
#define arvoreB_h
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"


//typedef long TipoChave;
/*
typedef struct TipoRegistro{
  //  TipoChave chave;
    Registro dados;
}TipoRegistro;*/

typedef struct TipoPagina{
    short n;//indica a quantidade de registro (itens) que tem dentro de cada página
    TipoRegistro r[MM];//vetor de duas posições, onde cada posição guarda um registro
    long p[MM + 1];//esse vetor contem +1, pois serve para indicar que são os filhos. 
}TipoPagina;

/* Função responsável por escrever na página do arquivo. 
   Parâmetros:
   - arquivoB: O arquivo onde a página será escrita.
   - pagina: A página a ser escrita no arquivo.
   - pos: A posição onde a página será escrita no arquivo. */
void escrevePagina(FILE *arquivoB,TipoPagina* pagina,long pos, TipoContador *cont);

/* Função responsável por ler a página do arquivo.
   Parâmetros:
   - arquivoB: O arquivo de onde a página será lida.
   - pagina: A página a ser lida do arquivo.
   - pos: A posição de onde a página será lida no arquivo. */
void lerPagina(FILE *arquivoB,TipoPagina* pagina,long pos, TipoContador *cont);

/* Função responsável por inicializar as páginas do arquivo.
   Inicializa a quantidade de itens de cada página como 0, os filhos como NULL (-1) e as chaves com -1 para evitar lixo. 
   Parâmetro:
   - raiz: A página que será inicializada. */
void inicializa ( TipoPagina *raiz);

/* Função responsável por ler do arquivo de origem (arq) e inserir no arquivo com a estrutura de uma árvore B.
   Parâmetros:
   - arq: O arquivo de origem.
   - arvoreB: O arquivo onde será implementada a árvore B.
   - quantidade: A quantidade de itens que serão inseridos no arquivo. */
void iniciarTreeB(FILE* arq, FILE*arvoreB, int quantidade,long *posRaiz, TipoContador *cont);

/* Função responsável por controlar a inserção nas páginas e na raiz.
   Verifica se a árvore precisará de uma nova raiz ou não (depende do que a função ins retornar).
   Parâmetros:
   - reg: O registro que será inserido.
   - pos: A posição onde o registro será inserido.
   - arvoreB: O arquivo da árvore B. */
void insereB( TipoRegistro reg,long *pos, FILE* arvoreB, TipoContador *cont);

/* Função responsável por verificar se a raiz está vazia ou não.
   Se estiver vazia, a função retorna para insereB e implementa a raiz. Caso contrário, verifica se a página tem espaço para receber mais um item. Se tiver espaço, insere na página; caso contrário, faz o overflow.
   Parâmetros:
   - reg: O registro que será inserido.
   - pos: A posição onde o registro será inserido.
   - cresceu: Indica se a árvore cresceu.
   - regRetorno: Registro de retorno após a inserção.
   - posRetorno: Posição de retorno após a inserção.
   - arvoreB: O arquivo da árvore B. */
void ins( TipoRegistro reg, long pos, short *cresceu,TipoRegistro *regRetorno,long *posRetorno, FILE *arvoreB, TipoContador *cont);

/* Função responsável por inserir um registro na página.
   Parâmetros:
   - pagina: A página onde o registro será inserido.
   - reg: O registro que será inserido.
   - apoDir: Apontador para a direita. */
void insereNaPagina ( TipoPagina *pagina,TipoRegistro reg,long apoDir, TipoContador *cont); 

/* Função responsável pela pesquisa no arquivo.
   Parâmetros:
   - reg: O registro a ser pesquisado.
   - pos: A posição onde a pesquisa será realizada.
   - arvoreB: O arquivo da árvore B.
   Retorna true se o registro for encontrado, caso contrário, false. */
bool pesquisaB ( TipoRegistro *reg,long pos,FILE *arvoreB, TipoContador *cont);
//void imprime(FILE*arvoreb, int quantidade);

#endif//arvoreB_h