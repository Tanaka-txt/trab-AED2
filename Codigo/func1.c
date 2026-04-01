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
  char *nomeLinha;//                                         ===== - bytes (Tem que remover 0 \0)
}reg_dados;

// BinarioNaTela();


// leitura csv, fazer filtro para interpretar cada coluna da tabela ",", filtrar condições se dado pode ser escrito

void read_csv(){

// =-=-= Abrimos o Arquivo CSV =-=-=
  FILE *estacoes = fopen("estacoes.csv", "r"); // csv
  // FILE *bin = fopen("saida.bin", "wb"); // bin escrevo nele 

  if (estacoes == NULL) { // Verifica se foi possível abrir arquivo
    printf("Erro na abertura");
    return;
  }

  char linha[256]; // ele começa no 256 pois ele pula a primeira linha com 255 caracteres + "\0". 256+256 =  512 (BUFFER!!!)
  char *ptr = linha; // loop linha += linha????
  reg_dados registro[1000];
  int i = 0;

  fgets(linha, 256, estacoes); // Pula o cabeçalho do csv

  while (fgets(linha, 256, estacoes)) { // faz o loop para leitura das linhas do csv
    char *ptr = linha;

    registro[i].codEstacao = atoi(strsep(&ptr, ","));
    registro[i].nomeEstacao = strsep(&ptr,",");
    registro[i].codLinha = atoi(strsep(&ptr,","));
    registro[i].nomeLinha = strsep(&ptr,",");
    registro[i].codProxEstacao = atoi(strsep(&ptr,","));
    registro[i].distProxEstacao = atoi(strsep(&ptr,","));
    registro[i].codLinhaIntegra = atoi(strsep(&ptr,","));
    registro[i].codEstIntegra = atoi(strsep(&ptr,","));

      // campo pode ser vazio
      char *campo = strsep(&ptr, ",");
      if (campo == NULL || strlen(campo) == 0)
          registro[i].codLinhaIntegra = -1;
      else
          registro[i].codLinhaIntegra = atoi(campo);

      campo = strsep(&ptr, ",");
      if (campo == NULL || strlen(campo) == 0)
          registro[i].codEstIntegra = -1;
      else
          registro[i].codEstIntegra = atoi(campo);

    // Debug
    printf("%d | %s | %d | %s | %d | %d | %d | %d\n", 
    registro[i].codEstacao,
    registro[i].nomeEstacao,
    registro[i].codLinha,
    registro[i].nomeLinha,
    registro[i].codProxEstacao,
    registro[i].distProxEstacao,
    registro[i].codLinhaIntegra,
    registro[i].codEstIntegra);
    
    i++;
  }

  

  // printf("nomeEstacao: %s\n", registro[1].nomeEstacao);
  // printf("codLinha: %d\n", registro[1].codLinha);
  // printf("nomeLinha: %s\n", registro[1].nomeLinha);
  // printf("codProxEstacao: %d\n", registro[1].codProxEstacao);
  // printf("distProxEstacao: %d\n", registro[1].distProxEstacao);
  // printf("codLinhaIntegra: %d\n", registro[1].codLinhaIntegra);
  // printf("codEstIntegra: %d\n", registro[1].codEstIntegra);


  // Fazer loop para leitura até o fim do arquivo
  // Dentro dele tem que fazer um loop para rodar por linha para anotar cada campo por registro
  // a,,c podemos usar para verificar esses vazios com RTSTOK???


  // fclose("estacoes.csv"); // Fecha arquivo
}

// escrever em um arquivo binário da forma esperada (atualizar o RRN do status)