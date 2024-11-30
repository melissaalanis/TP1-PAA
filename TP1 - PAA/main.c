#include <time.h>
#include "Labirinto.h"

#if MODO_ANALISE == 1
    extern int chamadas_recursivas;
    extern int nivel_maximo_recursao;
#endif

int main() {
    FILE *file_linha = fopen("Grafico/dados_grafico_linha.txt", "a");
    FILE *file_coluna = fopen("Grafico/dados_grafico_coluna.txt", "a");
    if (file_linha == NULL) {
        printf("Erro: arquivo file_linha não foi aberto corretamente.\n");
        return 1; // ou outro tratamento de erro
    }

    clock_t start_time, end_time;
    double elapsed_time;
    int opcao = 2;
    Posicao inicio;
    char nome_arquivo[30] = {0};
    int** labirinto;
    //int** matriz;
    printf("Bem-Vinda(o) \U0001F49A\n");
    
    while(opcao == 1 || opcao==2){  //O codigo entra em looping ate que o usuario queira sair. (opcao 3

        printf("Opcoes do labirinto:\n");
        printf("(1) Carregar novo arquivo de dados. \n");
        printf("(2) Processar e exibir resposta.\n");
        printf("(3 ou qualquer outro caracter) Sair do programa. \n");
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
                if(nome_arquivo == NULL){
                    printf("Por favor, carregue antes um arquivo de dados! \n");
                    printf("Pressione qualquer tecla para continuar... \n");
                    getchar(); 
                    getchar(); 
                }
                int linhas, colunas, chaves;

                // Captura o tempo inicial
                start_time = clock();
                processaLabirinto(nome_arquivo, &linhas, &colunas, &chaves, &inicio, &labirinto);
                
                // Vetor para armazenar o caminho
                Posicao* caminho = (Posicao*)malloc(linhas * colunas * sizeof(Posicao));
                int passos = 0;

                printf("Processando labirinto...\n\n");
                int ultima_coluna = 0;

                imprimeLabirinto(labirinto, linhas, colunas);

                if (!movimenta_estudante(labirinto, linhas, colunas, inicio.x, inicio.y, chaves, 0, caminho, &passos/*, &matriz*/)) {
                    printf("O estudante se movimentou %d e percebeu que o labirinto nao tem saida.\n", passos);
                } else {
                    printf("\nCaminho do estudante:\n\n");
                    for (int i = passos - 1; i >= 0; i--) {
                        printf("Linha: %d Coluna: %d\n", caminho[i].x, caminho[i].y);
                        ultima_coluna = caminho[i].y;
                       
                    }
                    printf("O estudante se movimentou %d vezes e chegou na coluna %d da primeira linha\n",passos, ultima_coluna);
                }

                #if MODO_ANALISE == 1
                    printf("Chamadas recursivas: %d\n", chamadas_recursivas);
                    printf("Nível máximo de recursividade: %d\n", nivel_maximo_recursao);
                #endif
                
                free(caminho);
                liberaLabirinto(labirinto, linhas);

                end_time = clock();
                elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

                fprintf(file_linha, "%d %f\n", linhas, elapsed_time);
                fprintf(file_coluna, "%d %f\n", colunas, elapsed_time);

                printf("Pressione Enter para continuar... \n");
                getchar(); // To consume the newline character left by previous input
                getchar(); // To wait for the Enter key
                break;


            default:
                printf("Saindo.... \n");
                sleep(1);
                printf("FIM.\n");
                break;
        }
        
        system("clear");  
        
    }
    // Fecha os arquivos
    fclose(file_linha);
    fclose(file_coluna);
    return 0;
}
