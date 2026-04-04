#include "features.h"

// Definição de Var


// Pares Estações
typedef struct {
    int EstacaoA;
    int EstacaoB;
} Par;


// leitura csv, fazer filtro para interpretar cada coluna da tabela ",", filtrar condições se dado pode ser escrito

// Cria Cabeçalho
reg_cabecalho cabecalho;


void write_registro_bin(reg_dados dados, FILE *binario); // Chamando ele aqui para usar no read csv


int ultimoRRN = 0; // tem que apontar para uma atualização do RRN

// Nro estacoes 👀
char **estacao = NULL;
int total_estacoes = 0;

// Pares Estações 👀
Par *pares = NULL;
int total_pares = 0;

void create_cabecalho(){
  cabecalho.status = '1';
  cabecalho.topo = -1;
  cabecalho.proxRRN = 0; // Comeã em 0, 0+1 = 1
  cabecalho.nroEstacoes = 0;
  cabecalho.nroParesEstacoes = 0;
}
//                 RegEstacoes   Estacoes   totalEstacoes
int existe_estacao(char *nome, char **lista, int tamanho){ // função de busca!👀
    for(int i = 0; i < tamanho; i++){  // Faz um loop com o total de estacoes👀
        if(strcmp(nome, lista[i]) == 0) // Verifica o nome da estação atual com o a lista de nome de estações👀
            return 1;
    }
    return 0;
}

//Pares Estações 👀
int existe_par(int a, int b, Par *lista, int tamanho){ //👀
  for(int i = 0; i < tamanho; i++){ // 👀
    if(lista[i].EstacaoA == a && lista[i].EstacaoB == b) // 👀
        return 1; // 👀
  }
    return 0; // 👀
}

void read_csv(char arq_csv[256], char arq_bin[256]){
  // =-=-= Abrimos o Arquivo CSV e Binário=-=-=
  FILE *csv = fopen(arq_csv, "r"); // csv cria o arquivo e se for existente ele reescreve por cima
  FILE *binario = fopen(arq_bin, "wb+"); // cria do zero o arquivo e permite usar o fseek!

  if (csv == NULL || binario == NULL) { // Verifica se foi possível abrir arquivo
    printf("Falha no processamento do arquivo.");
    return;
  }

  // =-=-= Cabeçalho =-=-= 
  create_cabecalho();// Cria o cabeçalho
  // Escreve cabeçalho
  cabecalho.status = '0'; // agora inconsistente (pois estamos escrevendo)

  char linha[256]; // ele começa no 256 pois ele pula a primeira linha com 255 caracteres + "\0". 256+256 =  512 (BUFFER!!!)
  char *ptr = linha; // loop linha += linha????
  reg_dados registro;

  fgets(linha, 256, csv); // Pula o cabeçalho do csv

  fseek(binario, 0, SEEK_SET); //garante que estamos começando a escrever no byte 0 do arquivo!

  fwrite(&cabecalho.status, 1, 1, binario);
  fwrite(&cabecalho.topo, sizeof(int), 1, binario);
  fwrite(&cabecalho.proxRRN, sizeof(int), 1, binario);
  fwrite(&cabecalho.nroEstacoes, sizeof(int), 1, binario);
  fwrite(&cabecalho.nroParesEstacoes, sizeof(int), 1, binario);

  fseek(binario, 17, SEEK_SET); // garante que começa após o cabeçalho

  while (fgets(linha, 256, csv)) { // faz o loop para leitura das linhas do csv (Mudar o loop para verificar que é o fim do arquivo sem usar o EOF)
    char *ptr = linha;
    char *temp; // recebe a leitura dos campos variados para alocar memória de acordo com o necessário

    registro.status_removido = '0'; // status removido 0-não /1-sim (status de todo o registro quando é "inicializado")

    // - CodEstação
    temp = strsep(&ptr, ",");
    if (temp == NULL || strlen(temp) == 0) {
      printf("Falha no processamento do arquivo.\n");
      fclose(csv);
      fclose(binario);
      return;
    }
      registro.codEstacao = atoi(temp);

    // - Nome estação
    temp = strsep(&ptr, ",");
    registro.nomeEstacao = malloc(strlen(temp)+1); // faz leitura, coloca em temp e ve o tamanho e aloca o tamanho+1
    temp[strcspn(temp, "\n")] = '\0';// limpeza strcopy
    strcpy(registro.nomeEstacao, temp); // copia a informação obtida na var do registro
    registro.tamNomeEstacao = strlen(temp); // pega tamanho do nome da estação

    // - CodLinha
    temp = strsep(&ptr, ",");
    if(temp == NULL || strlen(temp) == 0){
      registro.codLinha = -1;
    } else {
      registro.codLinha = atoi(temp);
    };

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
    };

    // Dist Proxi Estac
    temp = strsep(&ptr, ",");
    if(temp == NULL || strlen(temp) == 0){
      registro.distProxEstacao = -1;
    } else {
      registro.distProxEstacao = atoi(temp);
    };

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
      if (temp != NULL) { //strlen("\n") retorna 1
        temp[strcspn(temp, "\r\n")] = '\0'; 
      }
    if (temp == NULL || strlen(temp) == 0){ //strlen("\n") retorna 1, não 0!  
      registro.codEstIntegra = -1;
    }
    else {
      registro.codEstIntegra = atoi(temp);
    };

    registro.prox_queue = cabecalho.topo;

    // Verifica nroEstacoes👀
    if (!existe_estacao(registro.nomeEstacao, estacao, total_estacoes)) { // 👀
        estacao = realloc(estacao, (total_estacoes + 1) * sizeof(char*)); // vai realocando a lista de estações com os nomes👀
        estacao[total_estacoes] = strdup(registro.nomeEstacao); // 👀
        total_estacoes++; // Incrementa 👀
    }
    cabecalho.nroEstacoes = total_estacoes;

    // Pares Estações👀
    if (registro.codProxEstacao != -1 && !existe_par(registro.codEstacao, registro.codProxEstacao, pares, total_pares)) { 
        pares = realloc(pares, (total_pares + 1) * sizeof(Par)); // Aqui é para realocar espaço para os nomes das estações👀
        pares[total_pares].EstacaoA = registro.codEstacao; // Aqui fica as CodEstação que está escrevendo qual é seu par👀
        pares[total_pares].EstacaoB = registro.codProxEstacao; // Aqui fica os códigos das estações pares👀
        total_pares++;
    }
    cabecalho.nroParesEstacoes = total_pares;

    write_registro_bin(registro, binario);

    ultimoRRN++; // add o rrn
    cabecalho.proxRRN = ultimoRRN;
    
  }
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

  fseek(binario, 0, SEEK_SET); //garante que estamos começando a escrever no byte 0 do arquivo!

  cabecalho.status = '1'; // agora consistente

  fwrite(&cabecalho.status, 1, 1, binario);
  fwrite(&cabecalho.topo, sizeof(int), 1, binario);
  fwrite(&cabecalho.proxRRN, sizeof(int), 1, binario);
  fwrite(&cabecalho.nroEstacoes, sizeof(int), 1, binario);
  fwrite(&cabecalho.nroParesEstacoes, sizeof(int), 1, binario);

  fclose(csv); // fechamos o CSV
  fclose(binario);

   // Libera memória

  free(pares);
  pares = NULL;
  total_pares = 0;

  free(registro.nomeEstacao);
  free(registro.nomeLinha);

  for(int i = 0; i < total_estacoes; i++){
    free(estacao[i]);
  }
  free(estacao);
  estacao = NULL;
  total_estacoes = 0;

  ultimoRRN = 0;

  BinarioNaTela(arq_bin);
}

// escrever em um arquivo binário da forma esperada (atualizar o RRN do status)

void write_registro_bin(reg_dados dados, FILE *binario){ // arquivo binário, para escrever
 // Verificação de tamanho com base no tamanho do nome
  int tamanho_variados = 37 + dados.tamNomeEstacao + dados.tamNomeLinha; // 37 bytes fixos + bytes variados pois vetores de char são 1 byte cada caracter
  int lixo = 80 - tamanho_variados; // lixo para ser completado com $
    if (tamanho_variados > 80) {
      printf("Erro: registro maior que 80 bytes\n");
      return;
    }
  
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
  fwrite(dados.nomeEstacao, 1, dados.tamNomeEstacao, binario);

  fwrite(&dados.tamNomeLinha, sizeof(int), 1, binario);  
  fwrite(dados.nomeLinha, 1, dados.tamNomeLinha, binario); 

  for(int i = 0; i < lixo; i++){
    char caracter_lixo = '$';
    fwrite(&caracter_lixo, 1, 1, binario); // Printa os lixos
  }
}

// Preciso escrever o cabeçalho no topo
// Verificar se removi o \0 antes de por no
// termina codigo
// Verifica um pouco comparando com o outro bin
// Adicionar numero estacoes
// numeros pares de estações

// nroEstacoes : Indica a quantidade de estações diferentes que são armazenadas no arquivo de dado. Note que se houver 2 ou mais estações com mesmo nome elas são as mesmas (Tamanho : inteiro = 4 bytes!)
// nroParesEstacoes : Indica a quantidade de pares (codEstacao, codProxEstacao) diferentes que estão armazenadas nos arquivos de dados. (Tamanho : inteiro = 4 bytes!) ❓
