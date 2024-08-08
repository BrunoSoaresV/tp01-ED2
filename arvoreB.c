#include "arvoreB.h"
#include "struct.h"
// Função para inicializar a raiz ou uma página qualquer
void inicializa(TipoPagina *raiz) {
    (*raiz).n = 0; // Inicializa a quantidade de registros na página com 0
    for (int i = 0; i <= MM; i++) {
        raiz->r[i].dados.chave = -1; // Define a chave de todos os registros como -1 (indicando que estão vazios)
        raiz->p[i] = -1; // Define todos os filhos como -1 (indicando que não existem)
    }
}

// Função para escrever uma página no arquivo em uma posição específica
void escrevePagina(FILE *arquivoB, TipoPagina* pagina, long pos, TipoContador *cont) {
    fseek(arquivoB, pos * sizeof(TipoPagina), SEEK_SET); // Move o ponteiro do arquivo para a posição especificada
    fwrite(pagina, sizeof(TipoPagina), 1, arquivoB); // Escreve a página no arquivo
    (cont->escrita)++;
}

// Função para ler uma página do arquivo em uma posição específica
void lerPagina(FILE *arquivoB, TipoPagina* pagina, long pos, TipoContador *cont) {
    fseek(arquivoB, pos * sizeof(TipoPagina), SEEK_SET); // Move o ponteiro do arquivo para a posição especificada
    fread(pagina, sizeof(TipoPagina), 1, arquivoB); // Lê a página do arquivo
    (cont->leitura)++;
}

// Função para inicializar a árvore B a partir de um arquivo de origem(arq) e escrever no arquivo da árvore B
void iniciarTreeB(FILE* arq, FILE* arvoreB, int quantidade, long *posRaiz, TipoContador *cont) {
    TipoRegistro reg;
    Registro dado;

    *posRaiz = -1; // Define a raiz como vazia inicialmente

    // Leitura e inserção dos registros
    for (int i = 0; i < quantidade; i++) {
        if ((fread(&dado, sizeof(Registro), 1, arq)) != 1) {
            printf("Erro ao ler dado do arquivo de origem (Árvore B)\n");
            return;
        }else{(cont->leitura)++;}

        reg.dados = dado;
        insereB(reg, posRaiz, arvoreB, cont); // Insere o registro na árvore B
        //printf("[iniciarTreeB] chave a ser inserida = %d\n", reg.dados.chave);
       //printf("[iniciarTreeB] posRaiz = %ld\n", *posRaiz);
        //imprime(arvoreB, quantidade);
      
    }
}

// Função para inserir um registro na árvore B
void insereB(TipoRegistro reg, long *pos, FILE* arvoreB, TipoContador *cont) {
    short cresceu = 0; // Flag para indicar se a árvore cresceu
    TipoRegistro regRetorno;
    long posRetorno = -1;
    TipoPagina novaPag;

    ins(reg, *pos, &cresceu, &regRetorno, &posRetorno, arvoreB, cont); // Insere o registro e verifica se houve crescimento
    if (cresceu) {
        novaPag.n = 1; // Inicializa a nova página
        novaPag.r[0] = regRetorno; // Define o registro de retorno na nova página
        novaPag.p[1] = posRetorno; // Define a posição de retorno na nova página
        novaPag.p[0] = *pos; // Define a posição original na nova página

        for (int i = 2; i <= MM; i++) {
            novaPag.p[i] = -1; // Inicializa os demais filhos como -1
        }

        fseek(arvoreB, 0, SEEK_END); // Move o ponteiro do arquivo para o final
        *pos = ftell(arvoreB) / sizeof(TipoPagina); // Calcula a nova posição
        escrevePagina(arvoreB, &novaPag, *pos, cont); // Escreve a nova página no arquivo
    }
}

// Função recursiva para inserir um registro na árvore B
void ins(TipoRegistro reg, long pos, short *cresceu, TipoRegistro *regRetorno, long *posRetorno, FILE *arvoreB, TipoContador *cont) {
    long i = 1;
    long j;
    TipoPagina pagina, pagTemp;
    long final;

    if (pos == -1) { // Caso base: posição vazia
        *cresceu = true; // Indica que a árvore cresceu
        *regRetorno = reg; // Define o registro de retorno como o registro a ser inserido
        *posRetorno = -1; // Define a posição de retorno como -1
        return;
    }

    lerPagina(arvoreB, &pagina, pos, cont); // Lê a página da posição fornecida

    // Procura a posição correta para inserção
    while (i < pagina.n && reg.dados.chave > pagina.r[i - 1].dados.chave) { 
        i++;
        (cont->compChave)++;
    }

    // Se a chave já existe, não cresce
    (cont->compChave)++;
    if (reg.dados.chave == pagina.r[i - 1].dados.chave) {
        *cresceu = false;
        return;
    }
    // Verifica para qual página filho vai ser chamada ( direita ou esquerda). Se a chave que eu estou querendo inserir for maior do que a da pagina atual, eu não entro no if, com isso vou para o lado direito, se o item for menor eu decremento o i, com isso vou para a pagina esquerda 
    (cont->compChave)++;
    if (reg.dados.chave < pagina.r[i - 1].dados.chave) {
        i--; 
    }
    // Inserção recursiva na página filha adequada
    ins(reg, pagina.p[i], cresceu, regRetorno, posRetorno, arvoreB,cont);

    // Se não cresceu, retorne
    if (!*cresceu) { return; }

    lerPagina(arvoreB, &pagina, pos, cont); // Releia a página após possível modificação

    // Se há espaço na página, insira diretamente
    if (pagina.n < MM) {
        insereNaPagina(&pagina, *regRetorno, *posRetorno, cont); // Insere o registro na página
        escrevePagina(arvoreB, &pagina, pos, cont); // Escreve a página atualizada no arquivo
        *cresceu = false;
        return;
    }

    // Caso contrário, ocorre overflow e a página precisa ser dividida
    fseek(arvoreB, 0, SEEK_END); // Move o ponteiro do arquivo para o final
    final = ftell(arvoreB) / sizeof(TipoPagina); // Calcula a posição da nova página
    inicializa(&pagTemp); // Inicializa a nova página temporária

    // Verifica se o registro deve ser inserido na página temporária ou na original, dependendo da posição. Ou seja, se o meu i for maior do que a metade da pagina, quer dizer que o meu item é um valor alto, dessa forma, ele tem que ser inserindo diretamente na pagina temporaria, caso ao contrario o item vai ser inserido na pagina original
    if (i < M + 1) {
        // Se o índice de inserção está na primeira metade da página original
        insereNaPagina(&pagTemp, pagina.r[MM - 1], pagina.p[MM], cont); // Move o último registro da página original para a temporária
        pagina.n--; // Decrementa o número de registros na página original
        insereNaPagina(&pagina, *regRetorno, *posRetorno, cont); // Insere o novo registro na página original
    } else {
        // Se o índice de inserção está na segunda metade da página original
        insereNaPagina(&pagTemp, *regRetorno, *posRetorno, cont); // Insere o novo registro diretamente na página temporária
    }

    // Transfere metade dos registros da página original para a temporária
    for (j = M + 2; j <= MM; j++) {
        insereNaPagina(&pagTemp, pagina.r[j - 1], pagina.p[j], cont); // Move os registros da segunda metade para a temporária
    }

    pagina.n = M; // Ajusta o número de registros na página original para a metade
    pagTemp.p[0] = pagina.p[M + 1]; // Define o primeiro filho da pagina temporaria como o filho apos a metade da original
    escrevePagina(arvoreB, &pagina, pos, cont); // Escreve a pagina original de volta ao arquivo
    escrevePagina(arvoreB, &pagTemp, final, cont); // Escreve a nova pagina no arquivo

    *regRetorno = pagina.r[M]; // Define o registro de retorno como o registro do meio
    *posRetorno = final; // Define a posicao de retorno como a posicao da nova pagina
}

// Funcao para inserir um registro em uma pagina especifica
void insereNaPagina(TipoPagina *pagina, TipoRegistro reg, long apoDir, TipoContador *cont) {
    short naoAchouPosicao;
    int k;
    k = pagina->n;
    naoAchouPosicao = (k > 0);

    // Desloca registros e filhos para abrir espaco para o novo registro
    while (naoAchouPosicao) {
        (cont->compChave)++;
        if (reg.dados.chave >= pagina->r[k - 1].dados.chave) {
            naoAchouPosicao = 0;
            break;
        }
        pagina->r[k] = pagina->r[k - 1]; // Move o registro para a direita
        pagina->p[k + 1] = pagina->p[k]; // Move o filho correspondente para a direita
        k--;
        if (k < 1) {
            naoAchouPosicao = 0;
        }
    }

    // Insere o novo registro e ajusta os filhos
    pagina->r[k] = reg;
    pagina->p[k + 1] = apoDir;
    pagina->n++; // Incrementa o numero de registros na pagina

}

bool pesquisaB(TipoRegistro *reg, long pos, FILE *arvoreB, TipoContador *cont) {
    TipoPagina pagina;
   // printf("\nPesquisando chave: %d\n", reg->dados.chave);
   // printf("Posicao atual: %ld\n", pos);
    long i = 1 ;

    if (pos == -1) { // Verifica se a posicao e invalida
        return false;
    }

    lerPagina(arvoreB, &pagina, pos, cont); // Le a pagina da posicao fornecida
    /*printf("Pagina lida: n = %d\n", pagina.n);
    for (int j = 0; j < pagina.n; j++) {
        printf("Chave %d na pagina [%ld] : %d\n", j+1,pos, pagina.r[j].dados.chave);
    }*/
    // Inicializa i para procurar a posicao correta
  
    while (i < pagina.n && reg->dados.chave > pagina.r[i - 1].dados.chave) {
        i++;
        (cont->compChave)++;
    }

    // Verifica se encontrou a chave4
    (cont->compChave)++;
    if (reg->dados.chave == pagina.r[i-1].dados.chave) {
       // printf("Chave encontrada: %d\n", reg->dados.chave);
        *reg = pagina.r[i-1];
        return true;
    }

    // Decide qual subarvore explorar em seguida
    (cont->compChave)++;
    if (reg->dados.chave < pagina.r[i-1].dados.chave) {
        //printf("Descendo para a subarvore esquerda\n");
        return pesquisaB(reg, pagina.p[i-1],arvoreB,cont);
    } else {
        //printf("Descendo para a subarvore direita\n");
        return pesquisaB(reg, pagina.p[i],arvoreB,cont);
    }
}
/*
void imprime(FILE*arvoreb, int quantidade){
    fseek(arvoreb, 0, SEEK_END);
    int q = ftell(arvoreb) / sizeof(TipoPagina);
    printf("[%s] q = %d\n", __func__, q);

    fseek(arvoreb, 0, SEEK_SET);
    TipoPagina pagina; 
    
    for(long i = 0; i < q; i++){
        if(fread(&pagina, sizeof(TipoPagina), 1, arvoreb) < 1){
            return;
        }
        printf("Pagina %ld:\n", i);
        printf("\tchaves: ");
        for(int j = 0; j < pagina.n; j++){
            printf("%d ", pagina.r[j].chave);
        }
        printf("| filhos: ");
        for(int j = 0; j < pagina.n + 1; j++){  
            printf("%ld ", pagina.p[j]);
        }
        printf("\n");
    }
    printf("\n---------------------------\n\n");
}
*/