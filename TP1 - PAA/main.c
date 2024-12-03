#include <time.h>
#include "Labirinto/Labirinto.h"
#include "Labirinto/Labirintoteste.h"

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
    Posicao inicio;
    char nome_arquivo[30] = "";
    int** labirinto;
    //int** matriz;

    printf("Bem-Vinda(o) \U0001F49A\n");
    
    while(opcao == 1 || opcao==2 || opcao==3){  //O codigo entra em looping ate que o usuario queira sair (opcao 4)

        printf("Opcoes do labirinto:\n");
        printf("(1) Carregar novo arquivo de dados. \n");
        printf("(2) Processar e exibir resposta.\n");
        printf("(3) Gerar um labirinto de teste (Extra).\n");
        printf("(4 ou qualquer outro caracter) Sair do programa. \n");
        printf("Digite um numero: ");
        scanf("%d",&opcao);
        printf("\n");

        switch (opcao) {
            case 1: 
                printf("Por favor digite o nome do arquivo: ");
                scanf("%s", nome_arquivo);
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

                int linhas, colunas, chaves;
                
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
                Posicao* caminho = (Posicao*)malloc(linhas * colunas * sizeof(Posicao));

                unsigned int passos = 0; // Contador de passos (usigned pois nao pode ser negativo e pode ser muito grande)
                int tamanhoCaminho = 0; // O tamanho do caminho que leva ate a saida
                int ultima_coluna = 0; 

                printf("Processando labirinto...\n\n");
                
                imprimeLabirinto(labirinto, linhas, colunas);

                int resultado = movimenta_estudante(labirinto, linhas, colunas, inicio.x, inicio.y, chaves, 0, caminho, &passos, &tamanhoCaminho); // 1 - tem saida | 0 - nao tem saida
                
                if (resultado == 0) {
                    printf("O estudante se movimentou %d e percebeu que o labirinto nao tem saida.\n", passos);
                } else {
                    printf("\nCaminho do estudante:\n\n");
                    for (int i = tamanhoCaminho - 1; i >= 0; i--) { // Exibe o caminho feito ate saida comecando do inicio e indo ate a linha 0
                        printf("Linha: %d Coluna: %d\n", caminho[i].x, caminho[i].y);
                        ultima_coluna = caminho[i].y;
                    }
                    printf("O estudante se movimentou %d vezes e chegou na coluna %d da primeira linha\n",passos, ultima_coluna);
                }

                #if MODO_ANALISE == 1 // Quando o valor for 1, o modo analise estara ativado
                    printf("Chamadas recursivas: %d\n", chamadas_recursivas);
                    printf("Nível máximo de recursividade: %d\n", nivel_maximo_recursao);
                #endif

                imprimeMatriz(labirinto, linhas, colunas);
                
                // Libera a memoria alocada
                free(caminho);
                liberaLabirinto(labirinto, linhas);

                
                end_time = clock(); // Captura o tempo final
                elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calcula o tempo total

                if(linhas == colunas){
                    fprintf(file_grafico, "%d %f\n", colunas, elapsed_time); // O grafico so eh gerado para labirintos quadrados
                }

                printf("Pressione Enter para continuar... \n");
                getchar(); 
                getchar(); 
                break;

            case 3:
                srand(time(NULL)); 
                int linhas_teste, colunas_teste, chaves_teste, portas_teste, chaves_caminho, dificuldade; // Dados que serao usados para gerar o labirinto
                char nomeArquivo[100];

                printf("Digite as seguintes informações do labirinto a ser gerado:\n");
                printf("- quantidade de linhas\n");
                printf("- quantidade de colunas\n");
                printf("- quantidade de chaves\n");
                printf("- quantidade de portas\n");
                printf("- quantidade chaves no caminho\n");
                printf("- nivel de dificuldade (1 a 3 - facil para dificil)\n");
                printf("(Na mesma linha, separados por espaço): ");
                scanf("%d %d %d %d %d %d", &linhas_teste, &colunas_teste, &chaves_teste, &portas_teste, &chaves_caminho, &dificuldade);

                printf("Digite o nome do arquivo que você deseja gerar. (Exemplo: 'teste.txt'): ");
                scanf("%s", nomeArquivo);

                if(geraLabirintoTeste(linhas_teste, colunas_teste, chaves_teste, portas_teste, chaves_caminho, nomeArquivo, dificuldade)){
                    printf("Erro ao gerar o labirinto de teste! Por favor, tente de novo! \n");
                    printf("Pressione qualquer tecla para continuar... \n"); // Se nenhum arquivo foi informado, volta para o menu
                    getchar(); 
                    getchar(); 
                    break;
                    break;
                }
                printf("Labirinto gerado e salvo no arquivo '%s'\n", nomeArquivo);
                sleep(3);

            default:
                printf("Saindo.... \n");
                sleep(1);
                printf("FIM.\n");
                break;
        }
        
        system("clear");  
        
    }

    // Fecha o arquivo do grafico
    fclose(file_grafico);
    return 0;
}
