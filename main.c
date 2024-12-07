#include <time.h>
#include "Labirinto/Labirinto.h"
#include "LabirintoTeste/LabirintoTeste.h"

#if MODO_ANALISE == 1 // Quando o valor for 1, o modo analise estara ativado
    extern int chamadas_recursivas;
    extern int nivel_maximo_recursao;
#endif


int main() {
    FILE *file_grafico = fopen("Grafico/dados_grafico.txt", "a"); // Abre e cria, se nao existir, o arquivo para escrever os dados que serao plotados no grafico
    if (file_grafico == NULL) {
        printf("Erro: arquivo para plotar o gráfico não foi aberto corretamente.\n"); 
        return 1; 
    }

    // Variaveis para medir o tempo de execucao
    clock_t start_time, end_time; 
    double elapsed_time;

    int opcao = 2;
    int linhas, colunas, chaves;
    unsigned int passos = 0; // Contador de passos (usigned pois nao pode ser negativo e pode ser muito grande)
    int tamanho_caminho = 0; // O tamanho do caminho que leva ate a saida
    int ultima_coluna = 0; 
    Posicao inicio;
    Posicao* caminho;
    char nome_arquivo[30] = "";
    int** labirinto = NULL;
    int** matriz = NULL;
    int resultado_labirinto = -1; // Indica que nunca foi calculado a saida

    printf("|-------------------------------------------------------|\n");
    printf("|                   Bem-Vindo(a) \U0001F49A                     |\n");
    printf("|-------------------------------------------------------|\n");
    
    while(opcao == 1 || opcao==2 || opcao==3 || opcao==4){  //O codigo entra em looping ate que o usuario queira sair (opcao 4)

        printf("| Opcoes do labirinto:                                  |\n");
        printf("|-------------------------------------------------------|\n");
        printf("| (1) Carregar novo arquivo de dados.                   |\n");
        printf("| (2) Processar e exibir resposta.                      |\n");
        printf("| (3) Gerar um labirinto de teste (Extra).              |\n");
        printf("| (4) Observar caminho feito pelo estudante (Extra).    |\n");
        printf("| (5 ou qualquer outro caracter) Sair do programa.      |\n");
        printf("|-------------------------------------------------------|\n");
        printf("\nDigite um numero: ");
        scanf("%d",&opcao);
        printf("\n");

        switch (opcao) {
            case 1: 
                printf("Por favor digite o nome do arquivo: ");
                scanf("%s", nome_arquivo);
                
                if(labirinto != NULL){
                    liberaLabirinto(labirinto, linhas);
                    if(matriz != NULL)
                        liberaLabirinto(matriz, linhas);
                        matriz = NULL;
                    free(caminho);
                    #if MODO_ANALISE == 1 // Quando o valor for 1, o modo analise estara ativado
                        chamadas_recursivas = 0;
                        nivel_maximo_recursao = 0;
                    #endif
                    labirinto = NULL; 
                }
                resultado_labirinto = -1; // Indica que nunca foi calculado a saida

                sleep(1);
                break;
            case 2:
                if(strcmp(nome_arquivo, "") == 0){ // Verifica se o nome do arquivo foi carregado
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                    getchar(); 
                    getchar(); 
                    break;
                }    
                
                if(resultado_labirinto == -1){
                    // Captura o tempo inicial
                    start_time = clock();
                    // Preenche os dados do labirinto 
                    if(processaLabirinto(nome_arquivo, &linhas, &colunas, &chaves, &inicio, &labirinto)){
                        printf("Por favor, carregue um novo arquivo de dados! \n");
                        printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                        getchar(); 
                        getchar(); 
                        break;
                    }

                    // Vetor para armazenar o caminho feito pelo estudante
                    caminho = (Posicao*)malloc(linhas * colunas * sizeof(Posicao));
                    passos = 0; // Contador de passos (usigned pois nao pode ser negativo e pode ser muito grande)
                    tamanho_caminho = 0; // O tamanho do caminho que leva ate a saida
                    ultima_coluna = 0; 

                    resultado_labirinto = movimenta_estudante(labirinto, linhas, colunas, inicio.x, inicio.y, chaves, 0, caminho, &passos, &tamanho_caminho); // 1 - tem saida | 0 - nao tem saida{
                    end_time = clock(); // Captura o tempo final
                    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calcula o tempo total

                    if(linhas == colunas){
                        fprintf(file_grafico, "%d %f\n", colunas, elapsed_time); // O grafico so eh gerado para labirintos quadrados
                    }
                } 

                printf("Processando labirinto...\n");

                if (resultado_labirinto == 0) {
                    imprimeCaminho(labirinto, linhas, colunas);
                    printf("\nO estudante se movimentou %d vezes e percebeu que o labirinto nao tem saida.\n", passos);
                } else {
                    printf("\nCaminho do estudante:\n\n");
                    for (int i = tamanho_caminho - 1; i >= 0; i--) { // Exibe o caminho feito ate saida comecando do inicio e indo ate a linha 0
                        printf("Linha: %d Coluna: %d\n", caminho[i].x, caminho[i].y);
                        ultima_coluna = caminho[i].y;
                    }
                    printf("\nO estudante se movimentou %d vezes e chegou na coluna %d da primeira linha\n\n",passos, ultima_coluna);
                    imprimeCaminho(labirinto, linhas, colunas);

                }

                #if MODO_ANALISE == 1 // Quando o valor for 1, o modo analise estara ativado
                    printf("\nChamadas recursivas: %d\n", chamadas_recursivas);
                    printf("Nível máximo de recursividade: %d\n\n", nivel_maximo_recursao);
                #endif
                
                
                printf("Pressione Enter para continuar... \n");
                getchar(); 
                getchar(); 
                break;

            case 3:
                srand(time(NULL)); 
                int linhas_teste, colunas_teste, chaves_teste, portas_teste, chaves_caminho, dificuldade, portal; // Dados que serao usados para gerar o labirinto
                char nome_arquivo[100];

                printf("Digite a quantidade de linhas: ");
                scanf("%d", &linhas_teste);
                printf("Digite a quantidade de colunas: ");
                scanf("%d", &colunas_teste);
                printf("Digite a quantidade de chaves: ");
                scanf("%d", &chaves_teste);
                printf("Digite a quantidade de portas: ");
                scanf("%d", &portas_teste);
                printf("Digite a quantidade de chaves no caminho: ");
                scanf("%d", &chaves_caminho);
                printf("Digite o nível de dificuldade (1 a 3 - fácil para difícil): ");
                scanf("%d", &dificuldade);
                printf("Digite a quantidade de portais (0 ou 1): ");
                scanf("%d", &portal);

                if (verificaLimites(linhas_teste, colunas_teste, portas_teste, chaves_caminho, portal, dificuldade, chaves_teste)) {
                    printf("Nao eh possivel gerar labirinto com esses valores. Por favor, tente de novo! \n");
                    printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                    getchar(); 
                    getchar(); 
                    break;
                }
                printf("Digite o nome do arquivo que você deseja gerar. (Exemplo: 'teste.txt'): ");
                scanf("%s", nome_arquivo);
               
                // Verifica se o nome do arquivo esta no formato correto
                if (!strstr(nome_arquivo, ".txt")) {
                    strcat(nome_arquivo, ".txt");
                }
                
                if(geraLabirintoTeste(linhas_teste, colunas_teste, chaves_teste, portas_teste, chaves_caminho, nome_arquivo, dificuldade, portal)){
                    printf("Erro ao gerar o labirinto de teste! Por favor, tente de novo! \n");
                    printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                    getchar(); 
                    getchar(); 
                    break;
                }


                printf("Labirinto gerado e salvo no arquivo '%s'\n", nome_arquivo);
                printf("Pressione Enter para continuar... \n");
                getchar(); 
                getchar(); 
                break;

            case 4:
                if (strcmp(nome_arquivo, "") == 0) { // Verifica se o nome do arquivo foi carregado
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                    getchar(); 
                    getchar(); 
                    break;
                }

                if(resultado_labirinto == -1){
                    // Captura o tempo inicial
                    start_time = clock();
                    // Preenche os dados do labirinto 
                    if(processaLabirinto(nome_arquivo, &linhas, &colunas, &chaves, &inicio, &labirinto)){
                        printf("Por favor, carregue um novo arquivo de dados! \n");
                        printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                        getchar(); 
                        getchar(); 
                        break;
                    }
                    // Vetor para armazenar o caminho feito pelo estudante
                    caminho = (Posicao*)malloc(linhas * colunas * sizeof(Posicao));
                    passos = 0; // Contador de passos (usigned pois nao pode ser negativo e pode ser muito grande)
                    tamanho_caminho = 0; // O tamanho do caminho que leva ate a saida
                    ultima_coluna = 0; 

                    resultado_labirinto = movimenta_estudante(labirinto, linhas, colunas, inicio.x, inicio.y, chaves, 0, caminho, &passos, &tamanho_caminho); // 1 - tem saida | 0 - nao tem saida{
                    end_time = clock(); // Captura o tempo final
                    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calcula o tempo total

                    if(linhas == colunas){
                        fprintf(file_grafico, "%d %f\n", colunas, elapsed_time); // O grafico so eh gerado para labirintos quadrados
                    }
                } 


                printf("Processando labirinto...\n\n");

                if(resultado_labirinto == 0){
                    imprimeCaminho(labirinto, linhas, colunas);
                    printf("\nO estudante se movimentou %d vezes e percebeu que o labirinto nao tem saida.\n\n", passos);
                } else {
                    // Preenche os dados do labirinto
                    processaLabirinto(nome_arquivo, &linhas, &colunas, &chaves, &inicio, &matriz);
                    passos = 0; // Contador de passos (usigned pois nao pode ser negativo e pode ser muito grande)

                    movimentaEstudanteMatriz(matriz, linhas, colunas, inicio.x, inicio.y, chaves, 0, &passos);
                }

                #if MODO_ANALISE == 1 // Quando o valor for 1, o modo analise estara ativado
                        printf("Chamadas recursivas: %d\n", chamadas_recursivas);
                        printf("Nível máximo de recursividade: %d\n\n", nivel_maximo_recursao);
                #endif

                printf("Pressione Enter para continuar... \n");
                getchar();
                getchar();
                break;

            default:
                printf("Saindo.... \n");
                sleep(1);
                printf("FIM.\n");
                break;
        }
        
        system("clear");  
        
    }

    // Libera a memoria alocada
    if(labirinto != NULL){
        liberaLabirinto(labirinto, linhas);
        if(matriz != NULL)
            liberaLabirinto(matriz, linhas);
        free(caminho);
    }
    // Fecha o arquivo do grafico
    fclose(file_grafico);
    return 0;
}
