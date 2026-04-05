// vou fazer uma busca por critério
// vou ter que abrir o arquivo, pular o cabeçalho e usar o while para ler linha por linha (80 em 80 bytes)

// mas agora, o usuário digita no terminal o que ele quer procurar
// 1 - o programa vai ler o que o usuário quer buscar (o nome do campo) e qual é o valor (o critério)
// 2 - vou fazer a leitura do tamanho do nome, o nome, o tamanho da linha e assim por diante igual a func2 e vou guardar nas variáveis
// 3 - antes de finalizar e printar igual a func2, eu vou precisar fazer um if de verificação, para saber se a informação lida para com o que está sendo buscado
//      se for igual, imprimo a linha igual a lógica da func2
//      se for diferente, não imprime nada e dou um fseeek para pular pra próxima linha

// vai continuar tendo um contator para saber quantas linhas passaram no filtro, se continuar 0, imprime a mensagem de erro

// n é o número de buscas que o usuário quer fazer, vou precisar de um laço for (int i = 0; i < n; i++), cada busca, lê o arquivo do começo ao fim
// dentro de cada busca, o usuário digita m, que é a quantidade de filtros


#include "features.h"
// com esse "painel", o usuário digita o que ele quer, o programa vai ler a palavra e mudar de 0 para 1 no que vai ser buscado
typedef struct {
    int busca_codEstacao;       int valor_codEstacao;
    int busca_nomeEstacao;      char valor_nomeEstacao[50];
    int busca_codLinha;         int valor_codLinha;
    int busca_nomeLinha;        char valor_nomeLinha[50];
    int busca_codProxEstacao;   int valor_codProxEstacao;
    int busca_distProxEstacao;  int valor_distProxEstacao;
    int busca_codLinhaIntegra;  int valor_codLinhaIntegra;
    int busca_codEstIntegra;    int valor_codEstIntegra;
}Painel_Busca;

void busca_bin(char *arq_bin){
    FILE *teste = fopen(arq_bin, "rb"); // rb --> read binary
    if (teste == NULL) { // Verifica se foi possível ler o arquivo binário
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int n; // quantidade de buscas que o usuário precisa fazer
    scanf ("%d", &n);

    for (int i = 0; i < n; i++){ // laço para cada busca, lê o arquivo do começo ao fim
        Painel_Busca painel = {0}; // zerando o painel de busca para o novo filtro       
            
        int m; 
        scanf("%d", &m); // quantidade de filtros a serem buscados

        for (int j = 0; j < m; j++){ // laço para ler o que o usuário digitou que depois vai ser buscado
            char palavra [30];   // declaro uma variável que vai receber a palavra que vai ser buscada
            scanf("%s", palavra);

            // strcmp vai comparar as strings e ver se são iguais ou não
            if (strcmp(palavra, "codEstacao") == 0) {
                painel.busca_codEstacao = 1; // significa que o usuário quer pesquisar isso
                scanf("%d", &painel.valor_codEstacao); // guarda o valor que ele quer buscar

            } else if (strcmp(palavra, "nomeEstacao") == 0) {
                painel.busca_nomeEstacao = 1;
                ScanQuoteString(painel.valor_nomeEstacao);

            } else if (strcmp(palavra, "codLinha") == 0) {
                painel.busca_codLinha = 1;
                scanf("%d", &painel.valor_codLinha);

            } else if (strcmp(palavra, "nomeLinha") == 0) {
                painel.busca_nomeLinha = 1;
                ScanQuoteString(painel.valor_nomeLinha);

            } else if (strcmp(palavra, "codProxEstacao") == 0) {
                painel.busca_codProxEstacao = 1;
                scanf("%d", &painel.valor_codProxEstacao);

            } else if (strcmp(palavra, "distProxEstacao") == 0) {
                painel.busca_distProxEstacao = 1;
                scanf("%d", &painel.valor_distProxEstacao);

            } else if (strcmp(palavra, "codLinhaIntegra") == 0) {
                painel.busca_codLinhaIntegra = 1;
                scanf("%d", &painel.valor_codLinhaIntegra);

            } else if (strcmp(palavra, "codEstIntegra") == 0) {
                painel.busca_codEstIntegra = 1;
                scanf("%d", &painel.valor_codEstIntegra);
            }
            // quando esse laço for finalizado, vamos saber o que o usuário quer buscar
            /* IDEIA DO LAÇO:   
                    lê os 'm' filtros que o usuário deseja aplicar na busca e o scanf funciona em duas etapas: 
                    1º - identifica a palavara (ex: "codLinha") e 2º - ativa a busca desse campo (= 1) e lê o valor desejado logo em seguida.
            */           
        }    
        
               
        // ----------- FUNCIONALIDADE 2 -----------
        fseek(teste, 17, SEEK_SET); // pula o cabeçalho para iniciar a leitura
        
        int registroValido = 0; // contador para ver quantas buscas deram certo 
        reg_dados registro;     // struct que guarda o que acabou de ser lido
    
        while (fread(&registro.status_removido, sizeof(char), 1, teste) == 1){
        
            if (registro.status_removido == '1'){ 
                fseek(teste, 79, SEEK_CUR);

            }else if (registro.status_removido == '0'){ 
                
                fseek(teste, 4, SEEK_CUR); // pula Próximo RRN

                fread(&registro.codEstacao, sizeof(int), 1, teste);
                fread(&registro.codLinha, sizeof(int), 1, teste);
                fread(&registro.codProxEstacao, sizeof(int), 1, teste);
                fread(&registro.distProxEstacao, sizeof(int), 1, teste);
                fread(&registro.codLinhaIntegra, sizeof(int), 1, teste);
                fread(&registro.codEstIntegra, sizeof(int), 1, teste);

                // ------- 👀
                fread(&registro.tamNomeEstacao, sizeof(int), 1, teste);
                if (registro.tamNomeEstacao > 0) { 
                    registro.nomeEstacao = malloc(registro.tamNomeEstacao + 1); 
                    fread(registro.nomeEstacao, sizeof(char), registro.tamNomeEstacao, teste);    
                    registro.nomeEstacao[registro.tamNomeEstacao] = '\0'; 
                }

                fread(&registro.tamNomeLinha, sizeof(int), 1, teste);
                if (registro.tamNomeLinha > 0) { 
                    registro.nomeLinha = malloc(registro.tamNomeLinha + 1); 
                    fread(registro.nomeLinha, sizeof(char), registro.tamNomeLinha, teste); 
                    registro.nomeLinha[registro.tamNomeLinha] = '\0'; 
                }
                // ------- 👀
                // -----------||----------||-----------
                

                int pontos = 0; // registro lido começa com zero no filtro

                if (painel.busca_codEstacao == 1 && registro.codEstacao == painel.valor_codEstacao) pontos ++; // se o filtro do campo for "ligado" E o valor do registro bater com o exigido, ganha 1 ponto.
                if (painel.busca_codLinha == 1 && registro.codLinha == painel.valor_codLinha) pontos ++;
                if (painel.busca_codProxEstacao == 1 && registro.codProxEstacao == painel.valor_codProxEstacao) pontos ++;
                if (painel.busca_distProxEstacao == 1 && registro.distProxEstacao == painel.valor_distProxEstacao) pontos ++;
                if (painel.busca_codLinhaIntegra == 1 && registro.codLinhaIntegra == painel.valor_codLinhaIntegra) pontos ++;
                if (painel.busca_codEstIntegra == 1 && registro.codEstIntegra == painel.valor_codEstIntegra) pontos ++;

                // para os textos, é preciso usar strcmp para realizar a comparação
                if (painel.busca_nomeEstacao == 1 && strcmp (registro.nomeEstacao, painel.valor_nomeEstacao) == 0) pontos ++;
                if (painel.busca_nomeLinha == 1 && strcmp (registro.nomeLinha, painel.valor_nomeLinha) == 0) pontos ++;

                if (pontos == m){ // se a quantidade de pontos for igual ao nº de exigências vai ser um registro válido
                    registroValido ++;

                    // se o registro tiver os pontos == m, ele vai ser impresso
                    printf("%d %s ", registro.codEstacao, registro.nomeEstacao); // nunca vão ser nulas, por isso não precisa de verificação

                    // campos fixos que tiverem o valor nulo, em vez de ser -1 tem q printar NULO
                    // campos variáveis que tiverem o valor nulo, devem printar NULO

                    if (registro.codLinha == -1) {
                        printf("NULO ");
                    } else{
                        printf("%d ", registro.codLinha);
                    }

                    if (registro.tamNomeLinha == 0){
                        printf("NULO ");
                    }else {
                        printf("%s ", registro.nomeLinha);
                    }

                    if (registro.codProxEstacao == -1){
                        printf("NULO ");
                    }else {
                        printf("%d ", registro.codProxEstacao);
                    }

                    if (registro.distProxEstacao == -1){
                        printf("NULO ");
                    }else {
                        printf("%d ", registro.distProxEstacao);
                    }

                    if (registro.codLinhaIntegra == -1){
                        printf("NULO ");
                    }else {
                        printf("%d ", registro.codLinhaIntegra);
                    }

                    if (registro.codEstIntegra == -1){
                        printf("NULO\n");
                    }else {
                        printf("%d\n", registro.codEstIntegra);
                    }
                
                }         
            
                int bytesrestantes = 80 - 37 - registro.tamNomeEstacao - registro.tamNomeLinha;
                fseek(teste, bytesrestantes, SEEK_CUR); // fseek(arquivo, tamanho_do_pulo_em_bytes_do_lixo, a_partir_da_posição_atual_do_cursor)
                
                // limpando a memória dos mallocs antes que o while rode novamente
                if (registro.tamNomeEstacao > 0) free(registro.nomeEstacao);
                if (registro.tamNomeLinha > 0) free(registro.nomeLinha);

                }
            }
            if (registroValido == 0){ // se não tiver nenhum registro válido, ou seja, todas as estações foram removidas
                printf("Registro inexistente.\n");
        }  
    }
    fclose(teste);
}