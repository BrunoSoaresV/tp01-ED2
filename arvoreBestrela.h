#ifndef arvoreBestrela_h
#define arvoreBestrela_h
#include "struct.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef enum {Interna,Externa} TipoIntExt;

typedef struct TipoPaginaB{
    TipoIntExt pt;
    union{
        struct{
            int ni;
            TipoChave ri[MM];
            long pi[MM + 1];
        }U0;
        struct{
            int ne;
            TipoRegistro re[MM];
        }U1;
    }UU;
    
}TipoPaginaB;

/* Função responsável por escrever na página do arquivo. 
   Parâmetros:
   - arvoreBestr: O arquivo onde a página será escrita.
   - pagina: A página a ser escrita no arquivo.
   - pos: A posição onde a página será escrita no arquivo. */
void escrevePaginaB(FILE *arvoreBestr,TipoPaginaB* pagina,long pos,TipoContador* cont);

/* Função responsável por ler a página do arquivo.
   Parâmetros:
   - arvoreBestr: O arquivo de onde a página será lida.
   - pagina: A página a ser lida do arquivo.
   - pos: A posição de onde a página será lida no arquivo. */
void lerPaginaB(FILE *arvoreBestr,TipoPaginaB* pagina,long pos,TipoContador* cont);

/* Função responsável por inicializar as páginas do arquivo.
   Inicializa a quantidade de itens de cada página como 0, os filhos como NULL (-1) e as chaves com -1 para evitar lixo. 
   Parâmetro:
   - raiz: A página que será inicializada. */
void inicializaExt(TipoPaginaB *raiz);
/* Função responsável por ler do arquivo de origem (arq) e inserir no arquivo com a estrutura de uma árvore B.
   Parâmetros:
   - arq: O arquivo de origem.
   - arvoreB: O arquivo onde será implementada a árvore B.
   - quantidade: A quantidade de itens que serão inseridos no arquivo. */
void iniciarTreeBEstrela(FILE* arq, FILE*arvoreBestr, int quantidade,long *posRaiz, TipoContador* cont);

/* Função responsável por controlar a inserção nas páginas e na raiz.
   Verifica se a árvore precisará de uma nova raiz ou não (depende do que a função ins retornar).
   Parâmetros:
   - reg: O registro que será inserido.
   - pos: A posição onde o registro será inserido.
   - arvoreB: O arquivo da árvore B. */
void insereBEstrela( TipoRegistro reg,long *pos, FILE* arvoreBestr, TipoContador* cont);

/* Função responsável por verificar se a raiz está vazia ou não.
   Se estiver vazia, a função retorna para insereB e implementa a raiz. Caso contrário, verifica se a página tem espaço para receber mais um item. Se tiver espaço, insere na página; caso contrário, faz o overflow.
   Parâmetros:
   - reg: O registro que será inserido.
   - pos: A posição onde o registro será inserido.
   - cresceu: Indica se a árvore cresceu.
   - regRetorno: Registro de retorno após a inserção.
   - posRetorno: Posição de retorno após a inserção.
   - arvoreB: O arquivo da árvore B. */
void insB(TipoRegistro reg, long pos, short *cresceu, TipoRegistro *regRetorno, long *posRetorno, FILE *arvoreB, TipoContador* cont);

/* Função responsável por inserir um registro na página.
   Parâmetros:
   - pagina: A página onde o registro será inserido.
   - reg: O registro que será inserido.
   - apoDir: Apontador para a direita. */

void inicializaInt(TipoPaginaB *raiz);

bool pesquisaBEstrela ( TipoRegistro *reg,long pos,FILE *arvoreBestr, TipoContador* cont);

void insereNaPaginaExt(TipoPaginaB *pagina, TipoRegistro reg, TipoContador* cont);

void insereNaPaginaInt(TipoPaginaB *pagina, TipoChave chave, long apoDir, TipoContador* cont);

//void imprimeB(FILE*arvoreBestr, int quantidade);
#endif//arvoreBestrela_h