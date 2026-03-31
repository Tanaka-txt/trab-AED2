#include "features.h"

void banana(){
  printf("Bananada");
}

// struct para reg. cabeçalho 17 bytes MAX
typedef struct registro_cabecalho {
  char status[1]; // duvida, não sei se o tipo ta certo ===== 1 bytes
  int topo;//                                           ===== 4 bytes
  int proxRRN;//                                        ===== 4 bytes
  int nroEstacoes;//                                    ===== 4 bytes
  int nroParesEstacoes;//                               ===== 4 bytes
} reg_cabecalho;


// struct para estrura de dados 80 bytes MAX
typedef struct registro_dados {
  char status_removido[1]; // status se foi removido 0 ou 1  ===== 1 bytes
  int prox_queue; // proximo RRN da fila                     ===== 4 bytes
  int codEstacao; //                                         ===== 4 bytes
  int codLinha;//                                            ===== 4 bytes
  int codProxEstacao;//                                      ===== 4 bytes
  int distProxEstacao;//                                     ===== 4 bytes
  int codLinhaIntegra; //                                    ===== 4 bytes
  int codEstIntegra; //                                      ===== 4 bytes
  int tamNomeEstacao; //                                     ===== 4 bytes
  char *nomeEstacao; //                                      ===== - bytes (Tem que remover 0 \0)
  int tamNomeLinha;//                                        ===== 4 bytes
  char *nomeLinha;//                                       ===== - bytes (Tem que remover 0 \0)
}reg_dados;

// BinarioNaTela();

// ____________________________________________________________________________________________________________________________________________________ _________________ _______________
//|                                                TAMANHO FIXO                                                                      | TAMANHO VARIADO | TAMANHO VARIADO | TAMANHO FIXO  |
//| removido | proximo | codEstacao | codLinha | codProxEstacao | distProxEstacao | codLinhaIntegra | codEstIntegra | tamNomeEstacao |   Nome Estacao  |  tamNomeLinha   |   Nome Linha  |




// leitura csv, fazer filtro para interpretar cada coluna da tabela ",", filtrar condições se dado pode ser escrito

void read_csv(){

  FILE *estacoes = fopen("estacoes.csv", "r"); // csv
  // FILE *bin = fopen("saida.bin", "wb"); // bin escrevo nele 

  if (estacoes == NULL) { // Verifica se foi possível abrir arquivo
    printf("Erro na abertura");
    return;
  }

  char linha[256]; // ele começa no 256 pois ele pula a primeira linha com 255 caracteres + "\0".
  fgets(linha, 256, estacoes); // faz leitura do arquivo .csv

  // Para teste de leitura do arquivo.csv
    // while (fgets(linha, 256, estacoes)) {
    //   printf("%s", linha); // só pra testar
    // }

  // char *campo = strtok(256,",");//Vamos receber o endereço de cada campo do registro separadamente, a gente usa o strtok para fazer leitura em partes de string, onde o delimitador é o separador strtok(caracteres da linha, token)
  while (fgets(linha, 256, estacoes)) {

    char *token;

    token = strtok(linha, ",");
    
    int codEstacao = atoi(token);

    token = strtok(NULL, ",");
    int codLinha = atoi(token);

    token = strtok(NULL, ",");
    char *nomeEstacao = token;

    token = strtok(NULL, ",");
    char *nomeLinha = token;

    printf("Cod: %d | Linha: %d | Estacao: %s | NomeLinha: %s\n",
           codEstacao, codLinha, nomeEstacao, nomeLinha);
  }


  // fclose("estacoes.csv"); // Fecha arquivo
}

// escrever em um arquivo binário da forma esperada (atualizar o RRN do status)