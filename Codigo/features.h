#ifndef FEATURES_H
#define FEATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declaração da função
void banana();

// func1 - protótipos
void BinarioNaTela(char *arquivo);
void read_csv(char *arq_csv, char *arq_bin);
void create_cabecalho();

// Func2- 
void read_bin(char arq_bin[256]);

// Fornecidas
void BinarioNaTela(char *arquivo);
void ScanQuoteString(char *str);

#endif