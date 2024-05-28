#ifndef STRUCT_H
#define STRUCT_H

#define ITENSPAGINA 4
// Definindo a estrutura do registro
typedef struct {
    int chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} Registro;


#endif