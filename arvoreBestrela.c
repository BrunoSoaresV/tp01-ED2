#include "arvoreBestrela.h"

// Função para inicializar a raiz ou uma página qualquer
void inicializaExt(TipoPaginaB *raiz) {
    raiz->pt = Externa;
    (*raiz).UU.U1.ne = 0; // Inicializa a quantidade de registros na página com 0
    for (int i = 0; i <= MM; i++) {
        raiz->UU.U1.re[i].dados.chave = -1; // Define a chave de todos os registros como -1 (indicando que estão vazios)
    }
}
void inicializaInt(TipoPaginaB *raiz) {
    raiz->pt = Interna;
    (*raiz).UU.U0.ni = 0; // Inicializa a quantidade de registros na página com 0
    for (int i = 0; i <= MM; i++) {
        raiz->UU.U0.ri[i] = -1; // Define a filho de todos os registros como -1 (indicando que estão vazios)
    }
}

// Função para escrever uma página no arquivo em uma posição específica
void escrevePaginaB(FILE *arquivoBestrela, TipoPaginaB* pagina, long pos, TipoContador  *cont) {
    fseek(arquivoBestrela, pos * sizeof(TipoPaginaB), SEEK_SET); // Move o ponteiro do arquivo para a posição especificada
    fwrite(pagina, sizeof(TipoPaginaB), 1, arquivoBestrela); // Escreve a página no arquivo
    (cont->escrita)++;
}

// Função para ler uma página do arquivo em uma posição específica
void lerPaginaB(FILE *arquivoBestrela, TipoPaginaB* pagina, long pos, TipoContador *cont) {
    fseek(arquivoBestrela, pos * sizeof(TipoPaginaB), SEEK_SET); // Move o ponteiro do arquivo para a posição especificada
    fread(pagina, sizeof(TipoPaginaB), 1, arquivoBestrela); // Lê a página do arquivo
    (cont->leitura)++;
}

// Função para inicializar a árvore B a partir de um arquivo de origem(arq) e escrever no arquivo da árvore B
void iniciarTreeBEstrela(FILE* arq, FILE* arvoreBestr, int quantidade, long *posRaiz, TipoContador *cont) {
    TipoRegistro reg;
    Registro dados;
    //printf("[iniciarTreeBEstrela] entrou na função");
    *posRaiz = -2; // Define a raiz como vazia inicialmente

    // Leitura e inserção dos registros
    for (int i = 0; i < quantidade; i++) {
        if ((fread(&dados, sizeof(Registro), 1, arq)) != 1) {
            printf("Erro ao ler dado do arquivo de origem (arvore B estrela)\n");
            return;
        }else{(cont->leitura)++;}
        
        reg.dados = dados;
        //printf("[iniciarTreeBEstrela] chave a ser inserida = %d\n", reg.dados.chave);
       // printf("[iniciarTreeBEstelas] posRaiz = %ld\n", *posRaiz);
        insereBEstrela(reg, posRaiz, arvoreBestr,cont); // Insere o registro na árvore B
       // imprime(arvoreBestr, quantidade);
    }
}

// Função para inserir um registro na árvore B
void insereBEstrela(TipoRegistro reg, long *pos, FILE* arvoreBestr, TipoContador *cont) {
    short cresceu = 0; // Flag para indicar se a árvore cresceu
    TipoRegistro regRetorno;
    long posRetorno = -1;
    TipoPaginaB novaPag;

    insB(reg, *pos, &cresceu, &regRetorno, &posRetorno, arvoreBestr,cont); // Insere o registro e verifica se houve crescimento
    if (cresceu) {
        if(*pos == -2){
            inicializaExt(&novaPag);
            novaPag.UU.U1.ne = 1;
            novaPag.UU.U1.re[0] = regRetorno;
        } else{
         //   printf("[insereB] entrou no else");
            novaPag.pt = Interna;
            novaPag.UU.U0.ni = 1; // Inicializa a nova página
            novaPag.UU.U0.ri[0] = regRetorno.dados.chave; // Define o registro de retorno na nova página
            novaPag.UU.U0.pi[1] = posRetorno; // Define a posição de retorno na nova página
            novaPag.UU.U0.pi[0] = *pos; // Define a posição original na nova página

            for (int i = 2; i <= MM; i++) {
                novaPag.UU.U0.pi[i] = -1; // Inicializa os demais filhos como -1
            }
        }


        fseek(arvoreBestr, 0, SEEK_END); // Move o ponteiro do arquivo para o final
        *pos = ftell(arvoreBestr) / sizeof(TipoPaginaB); // Calcula a nova posição
        escrevePaginaB(arvoreBestr, &novaPag, *pos,cont); // Escreve a nova página no arquivo

    }
}


// Função recursiva para inserir um registro na árvore B
void insB(TipoRegistro reg, long pos, short *cresceu, TipoRegistro* regRetorno, long *posRetorno, FILE *arvoreBestr, TipoContador* cont) {
    long i = 1;
    long j;
    TipoPaginaB pagina, pagTemp;
    long final;
   // printf("[ins]  pos = %ld\n", pos);
    if (pos == -1 || pos == -2) { // Caso base: posição vazia
        (*cresceu) = true; // Indica que a árvore cresceu
     //   *regRetorno. = reg.chave; // Define o registro de retorno como o registro a ser inserido
        (*regRetorno) = reg;
        (*posRetorno) = -1; // Define a posição de retorno como -1
        return;
    }

    lerPaginaB(arvoreBestr, &pagina, pos,cont); // Lê a página da posição fornecida
    if(pagina.pt == Interna){
        // Procura a posição correta para inserção
        while (i < pagina.UU.U0.ni && reg.dados.chave > pagina.UU.U0.ri[i - 1]) {
            i++;
            (cont->compChave)++;
        }

        // Se a chave já existe, não cresce
        (cont->compChave)++;
        if (reg.dados.chave == pagina.UU.U0.ri[ i - 1]) {
            *cresceu = false;
            return;
        }
        // Verifica para qual página filho vai ser chamada ( direita ou esquerda). Se a chave que eu estou querendo inserir for maior do que a da pagina atual, eu não entro no if, com isso vou para o lado direito, se o item for menor eu decremento o i, com isso vou para a pagina esquerda 
        (cont->compChave)++;
        if (reg.dados.chave < pagina.UU.U0.ri[i - 1]) {
            i--;
        }

        // Inserção recursiva na página filha adequada
        insB(reg, pagina.UU.U0.pi[i], cresceu, regRetorno, posRetorno, arvoreBestr,cont);

        // Se não cresceu, retorne
        if (!*cresceu) { return; }

        lerPaginaB(arvoreBestr, &pagina, pos,cont); // Releia a página após possível modificação

        // Se há espaço na página, insira diretamente
        if (pagina.UU.U0.ni < MM) {
            insereNaPaginaInt(&pagina, regRetorno->dados.chave, *posRetorno,cont); // Insere o registro na página
            escrevePaginaB(arvoreBestr, &pagina, pos,cont); // Escreve a página atualizada no arquivo
            *cresceu = false;
            return;
        }

        // Caso contrário, ocorre overflow e a página precisa ser dividida
        fseek(arvoreBestr, 0, SEEK_END); // Move o ponteiro do arquivo para o final
        final = ftell(arvoreBestr) / sizeof(TipoPaginaB); // Calcula a posição da nova página
        inicializaInt(&pagTemp); // Inicializa a nova página temporária

        // Verifica se o registro deve ser inserido na página temporária ou na original, dependendo da posição. Ou seja, se o meu i for maior do que a metade da pagina, quer dizer que o meu item é um valor alto, dessa forma, ele tem que ser inserindo diretamente na pagina temporaria, caso ao contrario o item vai ser inserido na pagina original
        if (i < M + 1) {
            // Se o índice de inserção está na primeira metade da página original
            insereNaPaginaInt(&pagTemp, pagina.UU.U0.ri[MM - 1], pagina.UU.U0.pi[MM],cont); // Move o último registro da página original para a temporária
            pagina.UU.U0.ni--; // Decrementa o número de registros na página original
            insereNaPaginaInt(&pagina, regRetorno->dados.chave, *posRetorno,cont); // Insere o novo registro na página original
        } else {
            // Se o díndice de inserção está na segunda metade da página original
            insereNaPaginaInt(&pagTemp, regRetorno->dados.chave, *posRetorno,cont); // Insere o novo registro diretamente na página temporária
        }

        // Transfere metade dos registros da página original para a temporária
        for (j = M + 2; j <= MM; j++) {
            insereNaPaginaInt(&pagTemp, pagina.UU.U0.ri[j - 1], pagina.UU.U0.pi[j],cont); // Move os registros da segunda metade para a temporária
        }

        pagina.UU.U0.ni = M; // Ajusta o número de registros na página original para a metade
        pagTemp.UU.U0.pi[0] = pagina.UU.U0.pi[M + 1]; // Define o primeiro filho da página temporária como o filho após a metade da original
        escrevePaginaB(arvoreBestr, &pagina, pos,cont); // Escreve a página original de volta ao arquivo
        escrevePaginaB(arvoreBestr, &pagTemp, final,cont); // Escreve a nova página no arquivo
       // printf("[ins] pagTemp.n interna = %d\n", pagTemp.UU.U0.ni);
        regRetorno->dados.chave = pagina.UU.U0.ri[M]; // Define o registro de retorno como o registro do meio
        *posRetorno = final; // Define a posição de retorno como a posição da nova página
      //  printf("[ins] chaveRetorno = %d\n", regRetorno->chave);
      //  printf("[ins] posRetorno = %ld\n", *posRetorno);
       // printf("[ins] pagTemp.UU.U0.pi[0] = %ld\n",pagTemp.UU.U0.pi[0]);
    }
    if (pagina.pt == Externa)
    {     
        while (i < pagina.UU.U1.ne && reg.dados.chave > pagina.UU.U1.re[i - 1].dados.chave){ 
            i++; 
            (cont->compChave)++;
        }
        //lerPaginaB(arvoreBestr, &pagina, pos); // Releia a página após possível modificação
        // Se há espaço na página, insira diretamente
        if (pagina.UU.U1.ne < MM) {
          //  printf("[ins] Externa reg: %d\n[ins] regRetorno: %d\n",reg.chave,regRetorno->chave);
            insereNaPaginaExt(&pagina, reg,cont); // Insere o registro na página
            escrevePaginaB(arvoreBestr, &pagina, pos,cont); // Escreve a página atualizada no arquivo
            *cresceu = false;
            return;
        }

        // Caso contrário, ocorre overflow e a página precisa ser dividida
        fseek(arvoreBestr, 0, SEEK_END); // Move o ponteiro do arquivo para o final
        final = ftell(arvoreBestr) / sizeof(TipoPaginaB); // Calcula a posição da nova página
        inicializaExt(&pagTemp); // Inicializa a nova página temporária

        // Verifica se o registro deve ser inserido na página temporária ou na original, dependendo da posição. Ou seja, se o meu i for maior do que a metade da pagina, quer dizer que o meu item é um valor alto, dessa forma, ele tem que ser inserindo diretamente na pagina temporaria, caso ao contrario o item vai ser inserido na pagina original
        if (i < M + 1) {
            // Se o índice de inserção está na primeira metade da página original
            insereNaPaginaExt(&pagTemp, pagina.UU.U1.re[MM - 1],cont); // Move o último registro da página original para a temporária
            pagina.UU.U1.ne--; // Decrementa o número de registros na página original
            insereNaPaginaExt(&pagina, *regRetorno,cont); // Insere o novo registro na página original
         //   printf("[ins] *regRetorno.chave = %d\n", regRetorno->chave);
        } else {
            // Se o índice de inserção está na segunda metade da página original
            insereNaPaginaExt(&pagTemp, *regRetorno,cont); // Insere o novo registro diretamente na página temporária
        }

        // Transfere metade dos registros da página original para a temporária
        for (j = M + 1; j <= MM; j++) {
            insereNaPaginaExt(&pagTemp, pagina.UU.U1.re[j - 1],cont); // Move os registros da segunda metade para a temporária
        }

        pagina.UU.U1.ne = M; // Ajusta o número de registros na página original para a metade
    
        escrevePaginaB(arvoreBestr, &pagina, pos,cont); // Escreve a página original de volta ao arquivo
        escrevePaginaB(arvoreBestr, &pagTemp, final,cont); // Escreve a nova página no arquivo
      //   printf("[ins] pagTemp.n  externa = %d\n", pagina.UU.U1.ne);
        *regRetorno = pagina.UU.U1.re[M]; // Define o registro de retorno como o registro do meio
        *posRetorno = final; // Define a posição de retorno como a posição da nova página
        *cresceu = true;
    }
}

// Função para inserir um registro em uma página específica
void insereNaPaginaExt(TipoPaginaB *pagina, TipoRegistro reg, TipoContador *cont) {
    short naoAchouPosicao;
    int k;
    k = pagina->UU.U1.ne;
   // printf ("[insereNaPaginaExt] k: %d\n", k);
  //  printf ("[insereNaPaginaExt] reg.chave: %d\n", reg.chave);
    naoAchouPosicao = (k > 0);

    // Desloca registros para abrir espaço para o novo registro
    while (naoAchouPosicao) {
        (cont->compChave)++;
        if (reg.dados.chave >= pagina->UU.U1.re[k - 1].dados.chave) {
            naoAchouPosicao = 0;
            break;
        }        

        pagina->UU.U1.re[k] = pagina->UU.U1.re[k - 1]; // Move o registro para a direita
        k--;
        if (k < 1) {
            naoAchouPosicao = 0;
        }
    }

    // Insere o novo registro e ajusta os filhos
    pagina->UU.U1.re[k] = reg;
    pagina->UU.U1.ne++; // Incrementa o número de registros na página
}
// Função para inserir um registro em uma página específica
void insereNaPaginaInt(TipoPaginaB *pagina, TipoChave chave, long apoDir, TipoContador *cont) {
    short naoAchouPosicao;
    int k;
    k = pagina->UU.U0.ni;
    naoAchouPosicao = (k > 0);

    // Desloca registros e filhos para abrir espaço para o novo registro
    while (naoAchouPosicao) {
        if (chave >= pagina->UU.U0.ri[k - 1]) {
            naoAchouPosicao = 0;
            break;
        }
        pagina->UU.U0.ri[k] = pagina->UU.U0.ri[k - 1]; // Move o registro para a direita
        pagina->UU.U0.pi[k + 1] = pagina->UU.U0.pi[k]; // Move o filho correspondente para a direita
        k--;
        if (k < 1) {
            naoAchouPosicao = 0;
        }
    }

    // Insere o novo registro e ajusta os filhos
    pagina->UU.U0.ri[k] = chave;
    pagina->UU.U0.pi[k + 1] = apoDir;
    pagina->UU.U0.ni++; // Incrementa o número de registros na página
}

// Função para procurar um registro em especifico
bool pesquisaBEstrela(TipoRegistro *reg, long pos, FILE *arvoreBestr, TipoContador *cont) {
    TipoPaginaB pagina;
    long i;

    if (pos == -1) { // Verifica se a posição é inválida
        return false;
    }
    lerPaginaB(arvoreBestr, &pagina, pos,cont);

    i = 1;
    if (pagina.pt == Interna){
        while (i < pagina.UU.U0.ni && reg->dados.chave > pagina.UU.U0.ri[i - 1]){ 
            i++;
            (cont->compChave)++;    
         }
        (cont->compChave)++;
        if (reg->dados.chave < pagina.UU.U0.ri[i - 1])
        {
            return pesquisaBEstrela(reg,pagina.UU.U0.pi[i -1],arvoreBestr,cont);
        }else{
            return pesquisaBEstrela(reg,pagina.UU.U0.pi[i],arvoreBestr,cont);
        }
    }
    
    while( i < pagina.UU.U1.ne && reg->dados.chave > pagina.UU.U1.re[ i-1].dados.chave){ 
        i++;
        (cont->compChave)++;
    }
    (cont->compChave)++;
    if (reg->dados.chave == pagina.UU.U1.re[i-1].dados.chave)   
    {
        *reg = pagina.UU.U1.re[i-1];
        return true;
    }else{
        return false;
    }
}
/*
void imprime(FILE*arvoreBestr, int quantidade){
    fseek(arvoreBestr, 0, SEEK_END);
    int q = ftell(arvoreBestr) / sizeof(TipoPaginaB);
    printf("[%s] q = %d\n", __func__, q);

    fseek(arvoreBestr, 0, SEEK_SET);
    TipoPaginaB pagina; 
    
    for(long i = 0; i < q; i++){
        if(fread(&pagina, sizeof(TipoPaginaB), 1, arvoreBestr) < 1){
            return;
        }
        printf("Pagina %ld:\n", i);
        if(pagina.pt == Interna){
            printf("\tchaves: ");
            for(int j = 0; j < pagina.UU.U0.ni; j++){
                printf("%d ", pagina.UU.U0.ri[j]);
            }
            printf(" | filhos: ");
            for(int j = 0; j < pagina.UU.U0.ni+1; j++){
                printf("%ld ", pagina.UU.U0.pi[j]);
            }
        }else{
            printf("\tchaves: ");
            for(int j = 0; j < pagina.UU.U1.ne; j++){
                printf("%d ", pagina.UU.U1.re[j].dados.chave);
            }
        }
        
        printf("\n");
    }
    printf("\n---------------------------\n\n");
}
*/