#ifndef FEATURES_H
#define FEATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registro.h"

// func1 - protótipos
void BinarioNaTela(char *arquivo);
void create_regi_bin(char arq_csv[256], char arq_bin[256]);
void create_cabecalho();

// Func2- 
void read_bin(char *arq_bin);

// Func3-
int ler_registro(FILE *binario, reg_dados *registro);
void imprimir_registro(reg_dados *registro);
void busca_bin(char *arq_bin);

// Func4-
int busca_por_rrn(char *arquivo, int posi_relativa);

// Fornecidas
void BinarioNaTela(char *arquivo);
void ScanQuoteString(char *str);

void le_linha_csv(char *linha, reg_dados *registro, int topo);

#endif