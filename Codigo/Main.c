#include "features.h"

void main() {
  int option;
  do{
    scanf("%d", &option);
      switch (option){
        case 1 :
          /*Função*/
          // printf("1\n");
          // banana();
          read_csv();
          // recebe nome.csv nome.bin
          break;
        case 2 :
          /*Função*/
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