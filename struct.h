#ifndef STRUCT_H
#define STRUCT_H

#define ITENSPAGINA 1000
#define M 1000 //Minimo
#define MM (M*2) //Maximo

typedef int TipoChave;

// Definindo a estrutura do registro
typedef struct {
    TipoChave chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} Registro;

typedef struct TipoRegistro{
   // TipoChave chave;
    Registro dados;
}TipoRegistro;

typedef struct TipoContador{
    long int leitura;
    long int escrita;
    long int compChave;
    double tempo;
}TipoContador;


#endif