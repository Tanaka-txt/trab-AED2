#include "features.h"


void main() {
  int option;
  char arq_csv[256], arq_bin[256]; // buffer do tamanho de uma linha
  do{
    scanf("%d", &option);
      switch (option){
        case 1 :
          /*Função*/
          // printf("1\n");

          // Verificamos se a entrada é diferente de vazio
          if(fgets(arq_csv, sizeof(arq_csv), stdin) != NULL){
            arq_csv[strcspn(arq_csv, "\n")] = '\0'; // remove /n
          }
          if(fgets(arq_bin, sizeof(arq_bin), stdin) != NULL){
            arq_bin[strcspn(arq_bin, "\n")] = '\0'; // remove /n
          }
          create_cabecalho();
          read_csv(arq_csv, arq_bin);

          // recebe nome.csv nome.bin
          break;
        case 2 :
          /*Função*/
          // Ler nome do arquivo binário
          printf("2\n");
          break;
        case 3 :
          /*Função*/
          printf("3\n");
          break;
        case 4 :
          /*Função*/
          printf("4\n");
          break;
        case 0 :
          /*Exit*/
          printf("0\n");
          break;
        default:
        /* Caso que não é nenhum da erro ex. -1 -2  9 */
        printf("Erro\n");
          exit;
      }
  } while (option != 0);
}