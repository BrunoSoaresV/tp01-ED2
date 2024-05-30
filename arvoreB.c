#include "arvoreB.h"
#include "struct.h"

void inicializa ( TipoApontador *tree){
    *tree = NULL;
}
void iniciarTreeB(FILE* arq, int quantidade, TipoContador *cont){
    TipoPagina *tree;
    TipoRegistro reg;
    FILE* arvoreB;

    inicializa(&tree);
    if((arvoreB = fopen("arvoreB.bin", "wb")) == NULL){
        printf("Erro ao abrir arquivo para Árvore B");
        return;
    };
    Registro dado;
    for(int i = 0; i < quantidade; i++){
        if((fread(&dado, 1, sizeof(Registro), arq)) != 1){
            printf("Erro ao ler dado do arquivo de origem (Árvore B)\n");
            return 0;
            }
            reg.dados = dado;
            insereB(reg,tree,&cont);
        }
}

bool pesquisaB ( TipoRegistro *reg,TipoApontador apo,int *nivel,TipoContador* cont){
    long i = 1; 
    if( apo == NULL ){                                                                  
        return false;
    }

    while ( i < apo->n && reg->dados.chave > apo->r[i-1].dados.chave){  
        cont->compPag++;
        i++;
    }
    cont->compChave++;
    if (reg->dados.chave == apo->r[i-1].dados.chave){
        *reg = apo->r[i-1];
        return true;
    }
    (*nivel)++;
    if( reg->dados.chave < apo->r[i-1].dados.chave){    
        return pesquisaB(reg,apo->p[i-1],nivel,cont);
    }else{ 
        return pesquisaB(reg, apo->p[i],nivel,cont);
    
    }
    
}

void insereB( TipoRegistro reg,TipoApontador * apo,TipoContador *cont){
    cont->transfe++;
    short cresceu;
    TipoRegistro regRetorno;
    TipoPagina *apoRetorno,*apoTemp;
    ins(reg,*apo,&cresceu,&regRetorno,&apoRetorno);
    if(cresceu){
        apoTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
        apoTemp->n = 1;
        apoTemp->r[0] = regRetorno;
        apoTemp->p[1] = apoRetorno;
        apoTemp->p[0] = *apo;
        *apo = apoTemp;
    }
}

void insereNaPagina ( TipoApontador apo,TipoRegistro reg,TipoApontador apoDir){
    short naoAchouPosicao;
    int k;//serve para indicar o numero de intens que tem dentro de cada pag
    k = apo->n;
    naoAchouPosicao = (k > 0);

    while (naoAchouPosicao)
    {
        if(reg.dados.chave >= apo->r[k-1].dados.chave){
            naoAchouPosicao = false;
            break;
        }
        apo->r[k] = apo->r[k-1];
        apo->p[k+1] = apo->p[k];
        k--;
        if(k < 1){
            naoAchouPosicao = false;
        }
    }
    apo->r[k] = reg;
    apo->p[k + 1]= apoDir;
    apo->n++; 
   
}

void imprime2 ( TipoApontador tree,int nivel ){
    int i = 0;
    if( tree == NULL ){
        return;
    }
    printf("nível: %d\n", nivel);
    for(i= 0; i<tree->n;i++){
        printf("%ld\n",tree->r[i].dados.chave);
    }
    nivel++;
    for(i=0;i<tree->n;i++){
        imprime2(tree->p[i],nivel);  
    }
    imprime2(tree->p[i],nivel); 
}

void imprime( TipoApontador tree){
    int n =0;
    imprime2(tree,n);
}


void ins( TipoRegistro reg, TipoApontador apo, short *cresceu,TipoRegistro *regRetorno,TipoApontador *apoRetorno){
    long i = 1;
    long j;
    TipoApontador apTemp;
    if(apo == NULL){
        *cresceu = true;
        (*regRetorno) = reg;
        (*apoRetorno) = NULL;
        return;
    }

    while (i < apo->n && reg.dados.chave > apo->r[i-1].dados.chave){ i++;}

    if(reg.dados.chave == apo->r[i-1].dados.chave){
        printf("Erro: Registro ja esta presente\n");
        *cresceu = false;
        return;
    }

    if(reg.dados.chave < apo->r[i - 1].dados.chave ){ i--; }

    ins(reg,apo->p[i],cresceu,regRetorno,apoRetorno);

    if(!*cresceu){ return; }

    if(apo->n < MM){
        insereNaPagina(apo,*regRetorno,*apoRetorno);
        *cresceu = false;
        return; 
    }
    //overflow: pagina tem que ser dividida
    apTemp = (TipoApontador) malloc ( sizeof(TipoPagina));
    apTemp->n = 0; apTemp->p[0] = NULL;
    if(i < M + 1 ){
        insereNaPagina(apTemp,apo->r[MM-1], apo->p[MM]);
        apo->n--;
        insereNaPagina(apo,*regRetorno,*apoRetorno);
    }else{
        insereNaPagina( apTemp,*regRetorno,*apoRetorno);
    }
    for (j= M + 2; j <= MM; j++){
        insereNaPagina(apTemp,apo->r[j-1],apo->p[j]);
    }
    apo->n = M;
    apTemp->p[0] = apo->p[M+1];
    *regRetorno = apo->r[M];
    *apoRetorno = apTemp;
}

