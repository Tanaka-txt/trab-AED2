// vou precisar ler linha pro linha do arquivo binário, sabendo o tamanho exato de cada pedaço pra não ler informação errada.
// comando 2 estacao.bin --> tentar abrir o arquivo no modo de leitura (em C rb é usado para ler binários)
// verificar se o arquivo foi aberto ou não --> mensagem de erro

// vou precisar pular os 17 bytes de cabeçalho antes de começar a ler as estações (dá pra usar o fseek)

// lendo as linhas (registros) --> cada linha da tabela é um registro que tem 80 bytes
// por isso vou precisar colocar um laço de repetição pra ler um registro de 80 bytes por vez até o arquivo acabar

// o primeiro byte que você lê nessa linha de 80 bytes é o campo que diz se ela foi removida ou não
// se tiver o caractere '1', significa que essa estação foi "apagada" do sistema. Você ignora o resto dessa linha, não imprime nada, e pula para os próximos 80 bytes

// se a linha não for apagada (tem o '0') vamos ler cada campo dela --> os dados são lidos e tratados um por um
// antes do nome da estação tem um nº que indica o tamanho daquele nome --> se for 0 imprime NULO

// o espaço é fixo (80 bytes) mas tem variação no nome das estações, oq sobrar tem q ser preenchido com $
// o programa precisa ler a quant. de letras do nome e ignorar os $ na hora de imprimir na tela

    /*  " - Para cada linha válida, você vai imprimir os dados em uma única linha no terminal, separando tudo por um simples espaço em branco.
        - A ordem de impressão tem que ser exatamente esta: código da estação, nome da estação, código da linha, nome da linha, código da próxima estação, distância, código da integração e, por fim, a estação de integração.
        - Se o seu programa rodar o arquivo inteiro e não encontrar nenhuma linha válida para imprimir (seja porque o arquivo só tinha cabeçalho ou tudo estava apagado), você deve exibir a mensagem Registro inexistente.."
    */


// --> main() --> no case 2 tem que fazer um scanf pra ler o nome do arquivo binário e passar ele pra função 2 (ex: func2_select(nomeArquivo);)

#include "features.h"

void read_bin(char arq_bin[256]){
    FILE *teste = fopen(arq_bin, "rb"); // rb --> read binary
    if (teste == NULL) { // Verifica se foi possível ler o arquivo binário
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // pular os 17 bytes de cabeçalho tem 2 formas: com tamanho fixo ou baseado no tamanho de uma struct --> fseek("nome_arquivo", 17, SEEK_SET) 
    fseek(teste, 17, SEEK_SET); // ou seja, a partir do início (SEEK_SET) pula 17 bytes que é um tamanho fixo --> movendo para a posição 18º

    // preciso fazer um while para ler cada linha do arquivo, baseando no tamanho fixo de 80 bytes
    // consigo usar o fread para ir lendo o arquivo e o fseek pra pular 79 bytes quando o removido for 1

    int registroValido = 0;  // contator para verificar se um registro válido foi encontrado no arquivo
    char removido; // variável de 1 byte 
    
    // &removido    --> endereço de salvamento da informação, 
    // sizeof(char) --> tamanho da informação a ser lida no char (1 byte)
    // 1            --> qtd de elementos a serem lidas, ou seja, 1 elemento
    // teste        --> arquivo
    // == 1         --> verifica se conseguiu ler 1 elemento (se não, é porque o arquivo acabou - retorna 0 e o loop para)
    while (fread(&removido, sizeof(char), 1, teste) ==1){
    
        if (removido == '1'){ // significa que o registro foi apagado depois de ler o 1º byte
            fseek(teste, 79, SEEK_CUR); // como já leu o 1º pula o restante (79 bytes a partir da posição atual) indo para a próxima linha
        
        }else if (removido == '0'){ // significa que o registro é válido e não foi apagado, então ele precisar ler todos os bytes
            // pula mais 4 bytes pois não precisa imprimir o campo 'próximo'
            fseek(teste, 4, SEEK_CUR);

            // criar as variáveis para o bloco com os campos fixos --> cada uma ocupa 4 bytes
            int codEstacao, codLinha, codProxEstacao, distProxEstacao, codLinhaIntegra, codEstIntegra; 


            // leitura campo a campo, na ordem que aparece no arquivo --> 6 inteiros de 4 bytes
            fread(&codEstacao, sizeof(int), 1, teste);
            fread(&codLinha, sizeof(int), 1, teste);
            fread(&codProxEstacao, sizeof(int), 1, teste);
            fread(&distProxEstacao, sizeof(int), 1, teste);
            fread(&codLinhaIntegra, sizeof(int), 1, teste);
            fread(&codEstIntegra, sizeof(int), 1, teste);


            // criar as variáveis para o bloco com os campos variáveis
            int tamNomeEstacao;
            fread(&tamNomeEstacao, sizeof(int), 1, teste);

            char nomeEstacao[tamNomeEstacao + 1]; // +1 por conta do '\0'
            if (tamNomeEstacao > 0){ // lê as letras de uma vez só se for maior que 0
                fread(nomeEstacao, sizeof(char), tamNomeEstacao, teste);    
            }
            // nomeEstacao[tamNomeEstacao] = '\0'; // como se fosse um pare para a leitura --> isso é necessário?


            int tamNomeLinha;
            fread(&tamNomeLinha, sizeof(int), 1, teste);

            char nomeLinha[tamNomeLinha + 1]; // +1 por conta do '\0'
            if (tamNomeLinha > 0){ // lê as letras de uma vez só se for maior que 0
                fread(nomeLinha, sizeof(char), tamNomeLinha, teste); 
            }
            // nomeEstacao[tamNomeEstacao] = '\0'; // como se fosse um pare para a leitura --> isso é necessário?

            registroValido++; // se o programa encontrar uma estação que não foi apagada ele soma 1 no número de registro válido

            printf("%d %s ", codEstacao, nomeEstacao); // nunca vão ser nulas, por isso não precisa de verificação

            // campos fixos que tiverem o valor nulo, em vez de ser -1 tem q printar NULO
            // campos variáveis que tiverem o valor nulo, devem printar NULO

            if (codLinha == -1) {
                printf("NULO ");
            } else{
                printf("%d ", codLinha);
            }

            if (tamNomeLinha == 0){
                printf("NULO ");
            }else {
                printf("%s ", nomeLinha);
            }

            if (codProxEstacao == -1){
                printf("NULO ");
            }else {
                printf("%d ", codProxEstacao);
            }

            if (distProxEstacao == -1){
                printf("NULO ");
            }else {
                printf("%d ", distProxEstacao);
            }

            if (codLinhaIntegra == -1){
                printf("NULO ");
            }else {
                printf("%d ", codLinhaIntegra);
            }

            if (codEstIntegra == -1){
                printf("NULO ");
            }else {
                printf("%d\n", codEstIntegra);
            }

            // somando apenas os bytes fixos: 1 do removido, 4 do próximo, 24 do bloco fico, 4 do tamNomeEstacao e 4 tamNomeLinha = 37 bytes
            // dos 80 bytes, 37 são fixos e temos a quant. de letras da estação e linha, o resto é o lixo de memória ($) que vai ser pulado
            int bytesrestantes = 80 - 37 - tamNomeEstacao - tamNomeLinha;
            fseek(teste, bytesrestantes, SEEK_CUR); // fseek(arquivo, tamanho_do_pulo_em_bytes_do_lixo, a_partir_da_posição_atual_do_cursor)
        }
    }

    if (registroValido == 0){ // se não tiver nenhum registro válido, ou seja, todas as estações foram removidas
        printf("Registro inexistente.\n");
    }

//*** tem q removero \0?

    fclose(teste);
}

