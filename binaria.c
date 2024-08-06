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

    while(paginaAtual < totalPaginas){
        if(paginaAtual == totalPaginas - 1) qtdItensPag = quantidade - (paginaAtual*ITENSPAGINA);
        if(qtdItensPag == 0) qtdItensPag = ITENSPAGINA;

        if(fread(&dados, sizeof(Registro), qtdItensPag, origem) != qtdItensPag){
            
            printf("Quantidade de arquivos na página %d \n", qtdItensPag);
            printf("Erro fatal ao tentar ler dado no arquivo de origem\n");
            return false;
        }else ((*cont).leitura)++;

        for(int i = 0; i < qtdItensPag; i++) {
            if(!insere(dados[i], qtdItens, arvore, cont))
                printf("Erro na insere|binaria.c\n");

            qtdItens++;
            //Colocar um return booleano na insere, ou fazer a verificação de erro nela mesma?
        }
        paginaAtual++;
    }
    fclose(arvore);
    fclose(txtFile);
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


int buscaBinariaI(FILE* arvore, int posAtual, tNo no, int chave, Registro *dado, TipoContador *cont){
    
     // Loop enquanto a posição atual não for -1 (indicando o final da busca)
    while(posAtual != -1){
        // Move o ponteiro do arquivo para a posição atual do nó
        fseek(arvore, posAtual * sizeof(tNo), SEEK_SET);

        // Lê o nó da posição atual do arquivo
        if(fread(&no, sizeof(tNo), 1, arvore) == 0){
            printf("Erro na leitura do arquivo da árvore\n");
            return -1;
        }else (*cont).leitura++;


         // Se a chave do nó lido for igual à chave procurada
        if(no.dados.chave == chave){
            *dado = no.dados; // Armazena os dados do nó encontrado no ponteiro dado e retorna a posição atual
            return posAtual;
        }
        
        // Se a chave do nó lido for menor que a chave procurada
        if(no.dados.chave < chave){
            
            posAtual = no.dir; // Atualiza a posição atual para o filho direito
        }else{
             // Caso contrário, atualiza a posição atual para o filho esquerdo
            posAtual = no.esq;
        }

        (*cont).compChave += 2; // são feitas duas comparações entre chaves a cada iteração
    }

    printf("Elemento de chave %d não está presente no arquivo.\n", chave);
    return -1;
}