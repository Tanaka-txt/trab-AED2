#ifndef FEATURES_H
#define FEATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"

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

// Func4-
int func4(char *arquivo, int posi_relativa);

// Fornecidas
void BinarioNaTela(char *arquivo);
void ScanQuoteString(char *str);

#endif