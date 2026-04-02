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
  char status_removido[1]; // status se foi removido 0 ou 1  ===== 1 bytes
  int prox_queue; // proximo RRN da fila                     ===== 4 bytes
  int codEstacao; //                                         ===== 4 bytes
  int codLinha;//                                            ===== 4 bytes
  int codProxEstacao;//                                      ===== 4 bytes
  int distProxEstacao;//                                     ===== 4 bytes
  int codLinhaIntegra; //                                    ===== 4 bytes
  int codEstIntegra; //                                      ===== 4 bytes
  int tamNomeEstacao; //                                     ===== 4 bytes
  char *nomeEstacao; //                                      ===== - bytes (Tem que remover 0 \0) melhor alocar com malloc de acordo com seu tamanho
  int tamNomeLinha;//                                        ===== 4 bytes
  char *nomeLinha;//                                         ===== - bytes (Tem que remover 0 \0) melhor alocar com malloc de acordo com seu tamanho
}reg_dados;

// BinarioNaTela();


// leitura csv, fazer filtro para interpretar cada coluna da tabela ",", filtrar condições se dado pode ser escrito

reg_cabecalho cabecalho;
char *verifica_estacao[150]; // usado para verificar estações diferentes
char *verifica_pares[150]; // verifica pares de estações
char *cod_estacoes[150];
char *cod_proxEs[150];
void write_bin();
//  ________ _____________ _____________ _______________ __________________
// |        |             |             |               |                  |
// | Status |    topo     |   proxRRN   |  nroEstacoes  | nroParesEstacoes |
// |________|_____________|_____________|_______________|__________________|

int ultimoRRN = 0; // tem que apontar para uma atualização do RRN

void create_cabecalho(){
  cabecalho.status = '0';
  cabecalho.topo = -1;
  cabecalho.proxRRN = ultimoRRN+1; // Comeã em 0, 0+1 = 1
  cabecalho.nroEstacoes = 0;
  cabecalho.nroParesEstacoes = 0;
}


void read_csv(char arq_csv[256], char arq_bin[256]){

  // =-=-= Abrimos o Arquivo CSV =-=-=
  FILE *estacoes = fopen(arq_csv, "r"); // csv cria o arquivo e se for existente ele reescreve por cima
  // FILE *bin = fopen("saida.bin", "wb"); // bin escrevo nele 

  if (estacoes == NULL) { // Verifica se foi possível abrir arquivo
    printf("Erro na abertura");
    return;
  }

  char linha[256]; // ele começa no 256 pois ele pula a primeira linha com 255 caracteres + "\0". 256+256 =  512 (BUFFER!!!)
  char *ptr = linha; // loop linha += linha????
  reg_dados registro;
  int i = 0;

  fgets(linha, 256, estacoes); // Pula o cabeçalho do csv

  while (fgets(linha, 256, estacoes)) { // faz o loop para leitura das linhas do csv (Mudar o loop para verificar que é o fim do arquivo sem usar o EOF)
    char *ptr = linha;
    char *temp; // recebe a leitura dos campos variados para alocar memória de acordo com o necessário

    registro.status_removido[0] = '0'; // status removido 0-não /1-sim (status de todo o registro quando é "inicializado")

    // - CodEstação
    temp = strsep(&ptr, ",");
    registro.codEstacao = atoi(temp);
    if(registro.codEstacao % 2 == 0){
      cabecalho.nroParesEstacoes += 1;
    }
    cod_estacoes[i] = registro.codEstacao;


    // - Nome estação
    temp = strsep(&ptr, ",");
    registro.nomeEstacao = malloc(strlen(temp)+1); // faz leitura, coloca em temp e ve o tamanho e aloca o tamanho+1
    temp[strcspn(temp, "\n")] = '\0';// limpeza strcopy
    strcpy(registro.nomeEstacao, temp); // copia a informação obtida na var do registro

    registro.tamNomeEstacao = strlen(temp); // pega tamanho do nome da estação

      for(int i = 0; i < 100; i++){
        if(registro.nomeEstacao == i){
          break;
        }
        else{
          cabecalho.nroEstacoes++; // Soma se não estiver lá
        }
      }

    // - CodLinha
    temp = strsep(&ptr, ",");
    registro.codLinha = atoi(temp);

    // - Nome Linha
    temp = strsep(&ptr, ",");
    registro.nomeLinha = malloc(strlen(temp)+1); // faz leitura, coloca em temp e ve o tamanho e aloca o tamanho+1
    registro.tamNomeLinha = strlen(temp); // pega tamanho do nome da linha
    temp[strcspn(temp, "\n")] = '\0'; // lipeza buffer copy
    strcpy(registro.nomeLinha, temp); // copia a informação obtida na var do registro


    // - Cod Prox Estac
    temp = strsep(&ptr, ",");
    if(temp == NULL || strlen(temp) == 0){
      registro.codProxEstacao = -1;
    } else {
      registro.codProxEstacao = atoi(temp); 
      cod_proxEs[i] = registro.codProxEstacao;
    };

    // Dist Proxi Estac
    temp = strsep(&ptr, ",");
    registro.distProxEstacao = atoi(temp);

    // Cod Linha Inte
    temp = strsep(&ptr, ",");
    if (temp == NULL || strlen(temp) == 0){
      registro.codLinhaIntegra = -1;
    }
    else{
      registro.codLinhaIntegra = atoi(temp);
    };

    // Cod Est Integra
    temp = strsep(&ptr, ",");
    if (temp == NULL || strlen(temp) == 0){
      registro.codEstIntegra = -1;
    }
    else {
      registro.codEstIntegra = atoi(temp);
    };

    registro.prox_queue = cabecalho.topo;

    // Verifica pares
    for(int k = 0; k < 150; k++){
      for(int j = 0; j < 150; j++){
        if(verifica_estacao[k] == cod_proxEs[j]){
          cabecalho.nroParesEstacoes++; // Não verifiquei se tem repetidos!!!!!!
        }
      }
    }

    write_bin(registro, arq_bin);

    // Debug
    // printf("RemV | ProxRRN | CodEstacao | CodLinha | CodProxEsta | DistProxEsta | CodLinhaInt | CodEstaInt |TamNomeEsta |NomeEsta | TamNomeLin | NomeLin |\n");
    // printf("%s | %d | %d | %d | %d | %d |%d | %d | %d | %s | %d | %s\n",
    // registro.status_removido, // %c
    // registro.prox_queue, // %d
    // registro.codEstacao, // %d
    // registro.codLinha, // %d
    // registro.codProxEstacao, // %d
    // registro.distProxEstacao, // %d
    // registro.codLinhaIntegra, // %d
    // registro.codEstIntegra, // %d
    // registro.tamNomeEstacao, // %d
    // registro.nomeEstacao, // %s
    // registro.tamNomeLinha, // %d
    // registro.nomeLinha // %s
    // );

    // Preciso verificar o tamanho de cada registro de tamanho variado e oque sobrar eu completo com $, tenho que verificar aquilo de cocatenar se o campo for grande e tals
    
    i++;
    free(registro.nomeEstacao);
    free(registro.nomeLinha);
  }
  fclose(estacoes); // fechamos o CSV
}

// escrever em um arquivo binário da forma esperada (atualizar o RRN do status)

void write_bin(reg_dados dados, reg_cabecalho header, char arq_bin[256]){ // arquivo binário, para escrever
  FILE *binario = fopen(arq_bin, "wb"); // Cria/Reescreve o arq binario

  
  fwrite(&header.status, 1, 1, binario);
  fwrite(&header.topo, sizeof(int), 1, binario);
  fwrite(&header.proxRRN, sizeof(int), 1, binario);
  fwrite(&header.nroEstacoes, sizeof(int), 1, binario);
  fwrite(&header.nroParesEstacoes, sizeof(int), 1, binario);

  //fwrite(buffer ,            sizeof,numero, ponteiro); - escrita 
  fwrite(&dados.status_removido, 1, 1, binario); 
  fwrite(&dados.prox_queue, sizeof(int), 1, binario);  // 4 bytes e 1 elemento a ser lido 
  fwrite(&dados.codEstacao, sizeof(int), 1, binario); 
  fwrite(&dados.codLinha, sizeof(int), 1, binario); 
  fwrite(&dados.codProxEstacao, sizeof(int), 1, binario); 
  fwrite(&dados.distProxEstacao, sizeof(int), 1, binario); 
  fwrite(&dados.codLinhaIntegra, sizeof(int), 1, binario); 
  fwrite(&dados.codEstIntegra, sizeof(int), 1, binario); 

  fwrite(&dados.tamNomeEstacao, sizeof(int), 1, binario); 
  fwrite(&dados.nomeEstacao, dados.tamNomeEstacao, 1, binario);

  fwrite(&dados.tamNomeLinha, sizeof(int), 1, binario);  
  fwrite(&dados.nomeLinha, dados.tamNomeLinha, 1, binario); 

  // Verificação de tamanho com base no tamanho do nome
  int tamanho_variados = 37 + dados.tamNomeEstacao + dados.tamNomeLinha; // 37 bytes fixos + bytes variados pois vetores de char são 1 byte cada caracter
  int lixo = 80 - tamanho_variados; // lixo para ser completado com $

  for(int i = 0; i < lixo; i++){
    char caracter_lixo = "$";
    fwrite(&caracter_lixo, 1, 1, binario); // Printa os lixos
  }

  fclose(binario);
}

// Preciso escrever o cabeçalho no topo
// Verificar se removi o \0 antes de por no
// termina codigo
// Verifica um pouco comparando com o outro bin
// Adicionar numero estacoes
// numeros pares de estações

// nroEstacoes : Indica a quantidade de estações diferentes que são armazenadas no arquivo de dado. Note que se houver 2 ou mais estações com mesmo nome elas são as mesmas (Tamanho : inteiro = 4 bytes!)
// nroParesEstacoes : Indica a quantidade de pares (codEstacao, codProxEstacao) diferentes que estão armazenadas nos arquivos de dados. (Tamanho : inteiro = 4 bytes!) ❓
