#include <time.h>
#include "Labirinto/Labirinto.h"
#include "Labirinto/Labirintoteste.h"

#if MODO_ANALISE == 1
    extern int chamadas_recursivas;
    extern int nivel_maximo_recursao;
#endif

int main() {
    FILE *file_grafico = fopen("Grafico/dados_grafico.txt", "a");
    if (file_grafico == NULL) {
        printf("Erro: arquivo file_linha não foi aberto corretamente.\n");
        return 1; // ou outro tratamento de erro
    }

    clock_t start_time, end_time;
    double elapsed_time;
    int opcao = 2;
    Posicao inicio;
    char nome_arquivo[30] = "";
    int** labirinto;
    //int** matriz;
    printf("Bem-Vinda(o) \U0001F49A\n");
    
    while(opcao == 1 || opcao==2 || opcao==3){  //O codigo entra em looping ate que o usuario queira sair. (opcao 3

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
                if(strcmp(nome_arquivo, "") == 0){
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n");
                    getchar(); 
                    getchar(); 
                    break;
                }
                int linhas, colunas, chaves;
                
                // Captura o tempo inicial
                start_time = clock();
                processaLabirinto(nome_arquivo, &linhas, &colunas, &chaves, &inicio, &labirinto);
                int** visitados;  // Definição global para a matriz de visitados
                visitados = (int**) malloc(linhas * sizeof(int*));
                for (int i = 0; i < linhas; i++) {
                    visitados[i] = (int*) malloc(colunas * sizeof(int));
                }


                // Inicializar a matriz de visitados (todos os valores começam como 0)
                for (int i = 0; i < linhas; i++) {
                    for (int j = 0; j < colunas; j++) {
                        visitados[i][j] = 0;  // Não visitado
                    }
                }


                // Vetor para armazenar o caminho
                Posicao* caminho = (Posicao*)malloc(linhas * colunas * linhas * colunas * sizeof(Posicao));
                int passos = 0;

                printf("Processando labirinto...\n\n");
                int ultima_coluna = 0;

                imprimeLabirinto(labirinto, linhas, colunas);

                /*movimenta_estudante_matriz(matriz, linhas, colunas, inicio.x, inicio.y, chaves, 0, caminho, &passos)*/
                int resultado = movimenta_estudante(labirinto, linhas, colunas, inicio.x, inicio.y, chaves, 0, caminho, &passos, visitados);
                //printf("%d\n", resultado);
                if (resultado == 0) {
                    printf("O estudante se movimentou %d e percebeu que o labirinto nao tem saida.\n", passos);
                } else {
                    printf("\nCaminho do estudante:\n\n");
                    for (int i = passos - 1; i >= 0; i--) {
                        printf("Linha: %d Coluna: %d\n", caminho[i].x, caminho[i].y);
                        ultima_coluna = caminho[i].y;
                        //imprimeLabirinto(labirinto, linhas, colunas);
                       
                    }
                    printf("O estudante se movimentou %d vezes e chegou na coluna %d da primeira linha\n",passos, ultima_coluna);
                }

                imprimeMatriz(labirinto, linhas, colunas);

                #if MODO_ANALISE == 1
                    printf("Chamadas recursivas: %d\n", chamadas_recursivas);
                    printf("Nível máximo de recursividade: %d\n", nivel_maximo_recursao);
                #endif
                
                free(caminho);
                liberaLabirinto(labirinto, linhas);
                /*liberaLabirinto(matriz, linhas);*/

                end_time = clock();
                elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

                if(linhas == colunas){
                    fprintf(file_grafico, "%d %f\n", colunas, elapsed_time); // O grafico so é gerado para labirintos quadrados
                }

                printf("Pressione Enter para continuar... \n");
                getchar(); 
                getchar(); 
                break;
            case 3:
                srand(time(NULL)); 
                int linhas_teste, colunas_teste, chaves_teste, portas_teste, chaves_caminho, dificuldade;
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

                geraLabirintoTeste(linhas_teste, colunas_teste, chaves_teste, portas_teste, chaves_caminho, nomeArquivo, dificuldade);
                printf("Labirinto gerado e salvo no arquivo '%s'\n", nomeArquivo);
                sleep(3);
                //strcpy(nomeArquivo, "teste7.txt");

            default:
                printf("Saindo.... \n");
                sleep(1);
                printf("FIM.\n");
                break;
        }
        
        system("clear");  
        
    }
    // Fecha os arquivos
    fclose(file_grafico);
    return 0;
}
