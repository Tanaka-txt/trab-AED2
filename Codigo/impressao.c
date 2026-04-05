#include "features.h"

void imprimir_registro (reg_dados *registro){

    printf("%d %s ", registro->codEstacao, registro->nomeEstacao); // nunca vão ser nulas, por isso não precisa de verificação

    // campos fixos que tiverem o valor nulo, em vez de ser -1 tem q printar NULO
    // campos variáveis que tiverem o valor nulo, devem printar NULO

    if (registro->codLinha == -1) {
        printf("NULO ");
    } else{
        printf("%d ", registro->codLinha);
    }

    if (registro->tamNomeLinha == 0){
        printf("NULO ");
    }else {
        printf("%s ", registro->nomeLinha);
    }

    if (registro->codProxEstacao == -1){
        printf("NULO ");
    }else {
        printf("%d ", registro->codProxEstacao);
    }

    if (registro->distProxEstacao == -1){
        printf("NULO ");
    }else {
        printf("%d ", registro->distProxEstacao);
    }

    if (registro->codLinhaIntegra == -1){
        printf("NULO ");
    }else {
        printf("%d ", registro->codLinhaIntegra);
    }

    if (registro->codEstIntegra == -1){
        printf("NULO\n");
    }else {
        printf("%d\n", registro->codEstIntegra);
    }
}



