#include "features.h"


int main() {
  int option;
  char arq_csv[256], arq_bin[256]; // buffer do tamanho de uma linha
  do{
    scanf("%d", &option);
    getchar(); // limpa o buffer por conta do \n
      switch (option){
        case 1 :
          /*Função*/
          // printf("1\n");

          // Verificamos se a entrada é diferente de vazio
          scanf("%s %s", arq_csv, arq_bin);
          create_cabecalho();
          read_csv(arq_csv, arq_bin);
          // BinarioNaTela("estacoes.bin"); teste para ver 
          break;
        case 2 :
          /*Função*/
          // Ler nome do arquivo binário
          scanf("%s", arq_bin);
          void read_bin(arq_bin);
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
  return 1;
}