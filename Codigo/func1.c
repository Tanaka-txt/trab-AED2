#include "features.h"

void banana(){
  // printf("Bananada");
  int test3 = 4;
  int *test1; 
  int **test2; 


  // Ponteiro que recebe entedereço de teste3
  test1 = &test3; // Qualquer coisa que fizer no test1 afeta o test3

  // Ponteiro que recebe o valor do endereço de teste3
  *test1 = test3; // não altera o valor mas confirma o acesso ao test3

  // Recebo endereço de uma var que recebe endereços
  test2 = &test1; // aponta para o test1 que por ventura aponta para o 3 então qualquer alteração no test2 muda o 1 e muda o 3

  printf("Teste1 - ende: %d\n", *test1); // &var	"Me dê o endereço de onde essa variável mora." (*test1 = &test3)
  printf("Teste1 - valor: %d\n", *test1); // *ptr	"Vá até o endereço que está guardado aqui e me dê o valor." (*test1 = test3)
  printf("Teste2: %d\n", **test2); //**ptr	"Vá ao endereço guardado aqui, pegue o endereço que encontrar e veja o valor no final." (**test2 = &test1)
}

// struct para reg. cabeçalho 17 bytes MAX
typedef struct registro_cabecalho {
  char status; // duvida, não sei se o tipo ta certo ===== 1 bytes indica consistência do arquivo, 0 = inconscistente / 1 = conscistente [0 = quando escreve e 1 quando termina]
  int topo;//                                           ===== 4 bytes
  int proxRRN;//                                        ===== 4 bytes
  int nroEstacoes;//                                    ===== 4 bytes
  int nroParesEstacoes;//                               ===== 4 bytes
} reg_cabecalho;


// struct para estrura de dados 80 bytes MAX
typedef struct registro_dados {
  char status_removido[0]; // status se foi removido 0 ou 1  ===== 1 bytes
  int prox_queue; // proximo RRN da fila                     ===== 4 bytes
  int codEstacao; //                                         ===== 4 bytes
  int codLinha;//                                            ===== 4 bytes
  int codProxEstacao;//                                      ===== 4 bytes
  int distProxEstacao;//                                     ===== 4 bytes
  int codLinhaIntegra; //                                    ===== 4 bytes
  int codEstIntegra; //                                      ===== 4 bytes
  int tamNomeEstacao; //                                     ===== 4 bytes
  char nomeEstacao; //                                      ===== - bytes (Tem que remover 0 \0) melhor alocar com malloc de acordo com seu tamanho
  int tamNomeLinha;//                                        ===== 4 bytes
  char nomeLinha;//                                         ===== - bytes (Tem que remover 0 \0) melhor alocar com malloc de acordo com seu tamanho
}reg_dados;

// BinarioNaTela();


// leitura csv, fazer filtro para interpretar cada coluna da tabela ",", filtrar condições se dado pode ser escrito

reg_cabecalho cabecalho;

//  ________ _____________ _____________ _______________ __________________
// |        |             |             |               |                  |
// | Status |    topo     |   proxRRN   |  nroEstacoes  | nroParesEstacoes |
// |________|_____________|_____________|_______________|__________________|

int *ultimoRRN;

void create_cabecalho(){
  cabecalho.status = '0';
  cabecalho.topo = -1;
  cabecalho.proxRRN = *ultimoRRN+1; // Comeã em 0, 0+1 = 1
  cabecalho.nroEstacoes = 0;
  cabecalho.nroParesEstacoes = 0;
}


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

    registro[i].status_removido[0] = '0'; // status removido 0-não /1-sim (status de todo o registro quando é "inicializado")

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
  // fclose("estacoes.csv"); // fechamos o CSV


  // Fazer loop para leitura até o fim do arquivo
  // Dentro dele tem que fazer um loop para rodar por linha para anotar cada campo por registro
  // a,,c podemos usar para verificar esses vazios com RTSTOK???


  // fclose("estacoes.csv"); // Fecha arquivo
}

// escrever em um arquivo binário da forma esperada (atualizar o RRN do status)