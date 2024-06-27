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
void escrevePagina(FILE *arquivoB, TipoPagina* pagina, long pos) {
    fseek(arquivoB, pos * sizeof(TipoPagina), SEEK_SET); // Move o ponteiro do arquivo para a posição especificada
    fwrite(pagina, sizeof(TipoPagina), 1, arquivoB); // Escreve a página no arquivo
}

// Função para ler uma página do arquivo em uma posição específica
void lerPagina(FILE *arquivoB, TipoPagina* pagina, long pos) {
    fseek(arquivoB, pos * sizeof(TipoPagina), SEEK_SET); // Move o ponteiro do arquivo para a posição especificada
    fread(pagina, sizeof(TipoPagina), 1, arquivoB); // Lê a página do arquivo
}

// Função para inicializar a árvore B a partir de um arquivo de origem(arq) e escrever no arquivo da árvore B
void iniciarTreeB(FILE* arq, FILE* arvoreB, int quantidade, long *posRaiz) {
    TipoRegistro reg;
    Registro dado;

    *posRaiz = -1; // Define a raiz como vazia inicialmente

    // Leitura e inserção dos registros
    for (int i = 0; i < quantidade; i++) {
        if ((fread(&dado, sizeof(Registro), 1, arq)) != 1) {
            printf("Erro ao ler dado do arquivo de origem (Árvore B)\n");
            return;
        }
        reg.dados = dado;
        insereB(reg, posRaiz, arvoreB); // Insere o registro na árvore B
        imprime(arvoreB, quantidade);
    }
}

// Função para inserir um registro na árvore B
void insereB(TipoRegistro reg, long *pos, FILE* arvoreB) {
    short cresceu = 0; // Flag para indicar se a árvore cresceu
    TipoRegistro regRetorno;
    long posRetorno = -1;
    TipoPagina novaPag;

    ins(reg, *pos, &cresceu, &regRetorno, &posRetorno, arvoreB); // Insere o registro e verifica se houve crescimento
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
        escrevePagina(arvoreB, &novaPag, *pos); // Escreve a nova página no arquivo
    }
}

// Função recursiva para inserir um registro na árvore B
void ins(TipoRegistro reg, long pos, short *cresceu, TipoRegistro *regRetorno, long *posRetorno, FILE *arvoreB) {
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

    lerPagina(arvoreB, &pagina, pos); // Lê a página da posição fornecida

    // Procura a posição correta para inserção
    while (i < pagina.n && reg.dados.chave > pagina.r[i - 1].dados.chave) { i++; }

    // Se a chave já existe, não cresce
    if (reg.dados.chave == pagina.r[i - 1].dados.chave) {
        *cresceu = false;
        return;
    }
    // Verifica para qual página filho vai ser chamada ( direita ou esquerda). Se a chave que eu estou querendo inserir for maior do que a da pagina atual, eu não entro no if, com isso vou para o lado direito, se o item for menor eu decremento o i, com isso vou para a pagina esquerda 
    if (reg.dados.chave < pagina.r[i - 1].dados.chave) { i--; }

    // Inserção recursiva na página filha adequada
    ins(reg, pagina.p[i], cresceu, regRetorno, posRetorno, arvoreB);

    // Se não cresceu, retorne
    if (!*cresceu) { return; }

    lerPagina(arvoreB, &pagina, pos); // Releia a página após possível modificação

    // Se há espaço na página, insira diretamente
    if (pagina.n < MM) {
        insereNaPagina(&pagina, *regRetorno, *posRetorno); // Insere o registro na página
        escrevePagina(arvoreB, &pagina, pos); // Escreve a página atualizada no arquivo
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
        insereNaPagina(&pagTemp, pagina.r[MM - 1], pagina.p[MM]); // Move o último registro da página original para a temporária
        pagina.n--; // Decrementa o número de registros na página original
        insereNaPagina(&pagina, *regRetorno, *posRetorno); // Insere o novo registro na página original
    } else {
        // Se o índice de inserção está na segunda metade da página original
        insereNaPagina(&pagTemp, *regRetorno, *posRetorno); // Insere o novo registro diretamente na página temporária
    }

    // Transfere metade dos registros da página original para a temporária
    for (j = M + 2; j <= MM; j++) {
        insereNaPagina(&pagTemp, pagina.r[j - 1], pagina.p[j]); // Move os registros da segunda metade para a temporária
    }

    pagina.n = M; // Ajusta o número de registros na página original para a metade
    pagTemp.p[0] = pagina.p[M + 1]; // Define o primeiro filho da página temporária como o filho após a metade da original
    escrevePagina(arvoreB, &pagina, pos); // Escreve a página original de volta ao arquivo
    escrevePagina(arvoreB, &pagTemp, final); // Escreve a nova página no arquivo

    *regRetorno = pagina.r[M]; // Define o registro de retorno como o registro do meio
    *posRetorno = final; // Define a posição de retorno como a posição da nova página
}

// Função para inserir um registro em uma página específica
void insereNaPagina(TipoPagina *pagina, TipoRegistro reg, long apoDir) {
    short naoAchouPosicao;
    int k;
    k = pagina->n;
    naoAchouPosicao = (k > 0);

    // Desloca registros e filhos para abrir espaço para o novo registro
    while (naoAchouPosicao) {
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
    pagina->n++; // Incrementa o número de registros na página
}

// Função para procurar um registro em especifico
// Função para procurar um registro em especifico
bool pesquisaB(TipoRegistro *reg, long pos, FILE *arvoreB) {
    TipoPagina pagina;
    long i=1;

    if (pos == -1) { // Verifica se a posição é inválida
        return false;
    }

    lerPagina(arvoreB, &pagina, pos); // Lê a página da posição fornecida

    // Inicializa i para procurar a posição correta
  
    while (i < pagina.n && reg->dados.chave > pagina.r[i].dados.chave) {
        i++;
    }

    // Verifica se encontrou a chave
    if (i < pagina.n && reg->dados.chave == pagina.r[i].dados.chave) {
        *reg = pagina.r[i];
        return true;
    }

    // Decide qual subárvore explorar em seguida
    if (i < pagina.n && reg->dados.chave < pagina.r[i].dados.chave) {
        return pesquisaB(reg, pagina.p[i],arvoreB);
    } else {
        return pesquisaB(reg, pagina.p[i + 1],arvoreB);
    }
}

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
            printf("%d ", pagina.r[j].dados.chave);
        }
        printf("| filhos: ");
        for(int j = 0; j < pagina.n + 1; j++){  
            printf("%ld ", pagina.p[j]);
        }
        printf("\n");
    }
    printf("\n---------------------------\n\n");
}
