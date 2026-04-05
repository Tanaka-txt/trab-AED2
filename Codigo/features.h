#ifndef FEATURES_H
#define FEATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs (TAD)
// struct para reg. cabeçalho 17 bytes MAX
typedef struct registro_cabecalho {
  char status; // duvida, não sei se o tipo ta certo ===== 1 bytes indica consistência do arquivo, 0 = inconscistente / 1 = conscistente [0 = quando escreve e 1 quando termina]
  int topo;//                                           ===== 4 bytes
  int proxRRN;//                                        ===== 4 bytes
  int nroEstacoes;//                                    ===== 4 bytes
  int nroParesEstacoes;//                               ===== 4 bytes
} reg_cabecalho;

//  ________ _____________ _____________ _______________ __________________
// |        |             |             |               |                  |
// | Status |    topo     |   proxRRN   |  nroEstacoes  | nroParesEstacoes |
// |________|_____________|_____________|_______________|__________________|

// struct para estrura de dados 80 bytes MAX
typedef struct registro_dados {
  char status_removido; // status se foi removido 0 ou 1     ===== 1 bytes
  int prox_queue; // proximo RRN da fila                     ===== 4 bytes
  int codEstacao; //                                         ===== 4 bytes
  int codLinha;//                                            ===== 4 bytes
  int codProxEstacao;//                                      ===== 4 bytes
  int distProxEstacao;//                                     ===== 4 bytes
  int codLinhaIntegra; //                                    ===== 4 bytes
  int codEstIntegra; //                                      ===== 4 bytes
  int tamNomeEstacao; //                                     ===== 4 bytes
  char *nomeEstacao; //                                      ===== - bytes (Tem que remover 0 \0) melhor alocar com malloc de acordo com seu tamanho
  int tamNomeLinha;//                                        ===== 4 bytes
  char *nomeLinha;//                                         ===== - bytes (Tem que remover 0 \0) melhor alocar com malloc de acordo com seu tamanho
}reg_dados;


// declaração da função
void banana();

// func1 - protótipos
void BinarioNaTela(char *arquivo);
void read_csv(char *arq_csv, char *arq_bin);
void create_cabecalho();

// Func2- 
void read_bin(char *arq_bin);

// Func3-
int ler_registro(FILE *binario, reg_dados *registro);
void imprimir_registro(reg_dados *registro);
void busca_bin(char *arq_bin);

// Fornecidas
void BinarioNaTela(char *arquivo);
void ScanQuoteString(char *str);

#endif