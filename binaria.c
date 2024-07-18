//Criação de uma árvore binária não balanceada

#include "binaria.h"
#define TAM_NOME 60 //tamanho do nome dos arquivos, se necessário. Até o momento não foi necessário, pois é passado já como ponteiro


//Cria um novo nó na árvore
tNo criaNovoNo(Registro dados){
    tNo novoNo;
    novoNo.dados = dados;
    novoNo.esq = -1;
    novoNo.dir = -1;
    return novoNo;
}


int calcularQuantidadeRegistros(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    return tamanho / sizeof(Registro);
}


//Os elementos do arquivo são percorridos e inseridos no novo arquivo
// Inserir enquanto tiver registros e páginas
// a última página tem o total de registros menos o quanto já foram inseridos (qtd de páginas * qtd por página)
bool geraArquivoBinaria(FILE* origem, int quantidade, TipoContador *cont){
    Registro dados[ITENSPAGINA];
    FILE* arvore;
    FILE* txtFile;
    long tamanho;
    

    //São definidas algumas variáveis de controle de fluxo
    int qtdItens = 0, paginaAtual = 0; //quantidade de itens já inseridos. // Valor da página atual
    int qtdItensPag = ITENSPAGINA; //quantidade de itens na página atual. Pode ser diferente para a última página
    int totalPaginas = (quantidade + ITENSPAGINA - 1) / ITENSPAGINA; //total previsto de páginas. Usado para controlar quando o processo deve ser encerrado

    if((arvore = fopen("treefile.bin", "wb+")) == NULL){
        printf("Não conseguiu criar o arquivo para armazenar a árvore\n");
        return false;
    }

    if((txtFile = fopen("treefile.txt", "w+")) == NULL){
        printf("Não conseguiu criar o arquivo para armazenar a árvore\n");
        return false;
    }

    tamanho = calcularQuantidadeRegistros(origem);
    printf("O arquivo contém %ld registros\n\n", tamanho);

    printf("Gerando arquivo da árvore binária\n");

    while(paginaAtual < totalPaginas){
        if(paginaAtual == totalPaginas - 1) qtdItensPag = quantidade - (paginaAtual*ITENSPAGINA);
        if(qtdItensPag == 0) qtdItensPag = ITENSPAGINA;

        if(fread(&dados, sizeof(Registro), qtdItensPag, origem) != qtdItensPag){
            
            printf("Quantidade de arquivos na página %d \n", qtdItensPag);
            printf("Erro fatal ao tentar ler dado no arquivo de origem\n");
            return false;
        }else ((*cont).leitura)++;

        for(int i = 0; i < qtdItensPag; i++) {
            insere(dados[i], qtdItens, arvore, cont);

            qtdItens++;
            //Colocar um return booleano na insere, ou fazer a verificação de erro nela mesma?
        }
        paginaAtual++;
    }
    fclose(arvore);
    fclose(txtFile);
    printf("Arquivo árvore gerado com sucesso!\n");
    return true; //Arquivo árvore gerado com sucesso
}

bool insere(Registro dado, int qtdItens, FILE* arvore, TipoContador *cont){
    // o dado recebido é atribuído a um nó
    tNo novoNo, noAtual;
    novoNo = criaNovoNo(dado);
    rewind(arvore);

    // caso a árvore ainda não tenha itens, este será o primeiro item dela
    if(qtdItens == 0){
        if(fwrite(&novoNo, sizeof(tNo), 1, arvore) != 1) return false;
        (*cont).escrita++;
        return true;
    }

    
    // procura o local correto de inserção (à direita ou à esquerda)
    // o nó atual deve ser lido o tempo todo para motivos de comparação. O processo deve ocorrer até que ocorra erro ou que seja bem sucedido.
    // por isso, a melhor opção é um while

    bool dadoInserido = false;
    long posAtual;

    while(!dadoInserido){
        posAtual = ftell(arvore);

        // le o nó atual
        if (fread(&noAtual, sizeof(tNo), 1, arvore) != 1) {
            
            if (feof(arvore)) {
                printf("Reached end of file unexpectedly.\n");
            } else if (ferror(arvore)) {
                perror("Error reading from file");
            }
            return false;
        }else (*cont).leitura++;

        // compara se o novo é maior.
        (*cont).compChave++; // é feita apenas uma comparação entre chaves a cada iteração
        if(novoNo.dados.chave > noAtual.dados.chave){
            //Se sim, verifica se o atual tem um filho à direita. Se não tiver, insere
            if(noAtual.dir == -1){
                // atualiza o endereço do filho (fim do arquivo)
                noAtual.dir = qtdItens;
                // volta o ponteiro do arquivo
                fseek(arvore, posAtual, SEEK_SET);
                // grava o nó corrente atualizado
                if((fwrite(&noAtual, sizeof(tNo), 1, arvore)) != 1){
                    printf("Falha em sobrescrever dado no arquivo\n");
                    return false;
                }
                (*cont).escrita++;

                // grava o novo nó no fim do arquivo
                fseek(arvore, 0, SEEK_END);
                if((fwrite(&novoNo, sizeof(tNo), 1, arvore)) != 1){
                    printf("Falha em adicionar dado no arquivo\n");
                    return false;
                }
                (*cont).escrita++;
                dadoInserido = true;

            }else{ //Se tiver filho, se move para ele e continua a verificação
                fseek(arvore, noAtual.dir * sizeof(tNo), SEEK_SET);
            }
            
        }else if(novoNo.dados.chave < noAtual.dados.chave){
            //Se sim, verifica se o atual tem um filho à direita. Se não tiver, insere
            if(noAtual.esq == -1){
                // atualiza o endereço do filho (fim do arquivo)
                noAtual.esq = qtdItens;
                // volta o ponteiro do arquivo em uma posição
                fseek(arvore, posAtual, SEEK_SET);
                // grava o nó corrente atualizado
                if((fwrite(&noAtual, sizeof(tNo), 1, arvore)) != 1){
                    printf("Falha em sobrescrever dado no arquivo\n");
                    return false;
                }
                (*cont).escrita++;
                // grava o novo nó no fim do arquivo
                fseek(arvore, 0, SEEK_END);
                if((fwrite(&novoNo, sizeof(tNo), 1, arvore)) != 1){
                    printf("Falha em adicionar dado no arquivo\n");
                    return false;
                }
                (*cont).escrita++;
                dadoInserido = true;

            }else{ //Se tiver filho, se move para ele e continua a verificação
                fseek(arvore, noAtual.esq * sizeof(tNo), SEEK_SET);
            }

        }else{ // se não for maior nem menor, é igual. Logo, o processo é inválido
            printf("Não é possível executar o procedimento pois já existe um item com essa chave na árvore\n");
            return false;
        }

    }

    return true;
}


int buscaBinaria(FILE* arvore, int posAtual, tNo no, int chave, Registro *dado, long int *leit, int depth){
    printf("Depth: %d\n", depth);
    depth++;
    // move o ponteiro do arquivo para o nó que será lido
    fseek(arvore, posAtual * sizeof(tNo), SEEK_SET);

    // lê o dado armazenado na posição
    if(fread(&no, sizeof(tNo), 1, arvore) == 0){
        printf("Erro na leitura do arquivo da árvore\n");
        return -1;
    }else (*leit)++;

    // compara a chave lida com a chave buscada
    if(no.dados.chave == chave){
        *dado = no.dados;
        return posAtual;
    
    // se a chave buscada for maior, continua buscando à direita
    }else if(no.dados.chave < chave){
        if(no.dir == -1 ){ // não há mais onde buscar, o elemento não está presente
            printf("O elemento de chave %d não está presente no arquivo\n", chave);
            return -1;
        }else{
            return buscaBinaria(arvore, no.dir, no, chave, dado, leit, depth);
        }

    // se a chave buscada for menor, continua buscando à esquerda
    }else{
        if(no.esq == -1){ // não há mais onde buscar, o elemento não está presente
            printf("O elemento de chave %d não está presente no arquivo\n", chave);
            return -1;
        }
        return buscaBinaria(arvore, no.esq, no, chave, dado, leit, depth); 
    }
}

int buscaBinariaI(FILE* arvore, int posAtual, tNo no, int chave, Registro *dado, TipoContador *cont){
    
    while(posAtual != -1){
        fseek(arvore, posAtual * sizeof(tNo), SEEK_SET);

        if(fread(&no, sizeof(tNo), 1, arvore) == 0){
            printf("Erro na leitura do arquivo da árvore\n");
            return -1;
        }else (*cont).leitura++;

        if(no.dados.chave == chave){
            *dado = no.dados;
            return posAtual;
        }
        
        if(no.dados.chave < chave){
            
            posAtual = no.dir;
        }else{
            posAtual = no.esq;
        }

        (*cont).compChave += 2; // são feitas duas comparações entre chaves a cada iteração
    }

    printf("Elemento de chave %d não está presente no arquivo.\n", chave);
    return -1;
}

// Imprime a árvore binária em um arquivo, no formato texto
void imprimeArvore(FILE* arvore, int qtd){
    FILE* txtFile;
    tNo no;

    // abre o arquivo texto
    if((txtFile = fopen("treefile.txt", "w")) == NULL){
        printf("Erro ao abrir arquivo para leitura (imprimeArvore)\n");
    }

    // imprime os dados da árvore
    for(int i = 0; i < qtd; i++){
        if(fread(&no, 1, sizeof(tNo), arvore) == -1){
            printf("Erro lendo da árvore (imprimeArvore)\n");
        }
        fprintf(txtFile, "Registro %d:\n", i);
        fprintf(txtFile, "Chave: %d\n", no.dados.chave);
        fprintf(txtFile, "Dado1: %ld\n", no.dados.dado1);
        fprintf(txtFile, "Dado2: %.10s\n", no.dados.dado2);
        fprintf(txtFile, "Dado3: %.10s\n", no.dados.dado3);
        fprintf(txtFile, "Dir: %d\n", no.dir);
        fprintf(txtFile, "Esq: %d\n\n", no.esq);
    }
}