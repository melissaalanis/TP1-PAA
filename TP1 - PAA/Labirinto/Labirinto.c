

#include "Labirinto.h"

#if MODO_ANALISE == 1
    int chamadas_recursivas = 0;
    int nivel_maximo_recursao = 0;
#endif

// Função para alocar memória para o labirinto
int** alocaLabirinto(int linhas, int colunas) {
    int** labirinto = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        labirinto[i] = (int*)malloc(colunas * sizeof(int));
    }
    return labirinto;
}

void processaLabirinto(char nome_arquivo[], int* linhas, int* colunas, int* chaves, Posicao* inicio, int*** labirinto){
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Le as dimensoes e numero de chaves
    fscanf(arquivo, "%d %d %d", linhas, colunas, chaves);

    // Aloca e carrega o labirinto
    *labirinto = alocaLabirinto(*linhas, *colunas);

    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, "%1d", &(*labirinto)[i][j]);
            if ((*labirinto)[i][j] == 0) {
                inicio->x = i;
                inicio->y = j;
            }
        }
    }    
    fclose(arquivo);
}

void imprimeLabirinto(int** labirinto, int linhas, int colunas){
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            //printf("%d ", labirinto[i][j]);
            if(labirinto[i][j]==9){
                printf(cor_verde "%d " resetar_cor, labirinto[i][j]);
            }
            else if(labirinto[i][j]==0){
                 printf(cor_verde "%d " resetar_cor, labirinto[i][j]);
            }
            else if(labirinto[i][j]==2){
                 printf(cor_azul "%d " resetar_cor, labirinto[i][j]);
            }
            else if(labirinto[i][j]==3){
                 printf(cor_vermelha "%d " resetar_cor, labirinto[i][j]);
            }
            else if(labirinto[i][j]==4){
                 printf(cor_amarela "%d " resetar_cor, labirinto[i][j]);
            }
            else if(labirinto[i][j]==0){
                 printf(cor_verde "%d " resetar_cor, labirinto[i][j]);
            }
            else if(labirinto[i][j]==1){
                 printf(cor_branca "%d " resetar_cor, labirinto[i][j]);
            }
            
            else{
                printf("%d ", labirinto[i][j]);
            }
        }

        printf("\n");
    }
}

/*void** novaMatriz(int** labirinto, int linhas, int colunas) {  
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Le as dimensoes e numero de chaves
    fscanf(arquivo, "%d %d %d", linhas, colunas, chaves);

    // Aloca e carrega o labirinto
    *labirinto = alocaLabirinto(*linhas, *colunas);

    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            fscanf(arquivo, "%1d", &(*labirinto)[i][j]);
            if ((*labirinto)[i][j] == 0) {
                inicio->x = i;
                inicio->y = j;
            }
        }
    }    
    fclose(arquivo);
}*/

void imprimeMatriz(int** labirinto, int linhas, int colunas){
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            //printf("%d ", labirinto[i][j]);
            if(labirinto[i][j]==9){
                printf(cor_verde "  " resetar_cor);
            }
            else if(labirinto[i][j]==0){
                 printf(cor_verde "  " resetar_cor);
            }
            else if(labirinto[i][j]==2){
                 printf(cor_azul "  " resetar_cor);
            }
            else if(labirinto[i][j]==3){
                 printf(cor_vermelha "  " resetar_cor);
            }
            else if(labirinto[i][j]==4){
                 printf(cor_amarela "  " resetar_cor);
            }
            else if(labirinto[i][j]==0){
                 printf(cor_verde "  " resetar_cor);
            }
            else if(labirinto[i][j]==1){
                 printf(cor_branca "  " resetar_cor);
            }
        }

        printf("\n");
    }
}

// Função para liberar memória do labirinto
void liberaLabirinto(int** labirinto, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(labirinto[i]);
    }
    free(labirinto);
}

// Função para verificar se a posição é válida
int posicaoValida(int** labirinto, int linhas, int colunas, int x, int y, int chaves) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas) {
        if (labirinto[x][y] == 1 || labirinto[x][y] == 4) {
            return 1;
        } else if (labirinto[x][y] == 3 && chaves > 0) {
            return 1;
        }
    }
    return 0;
}

int movimenta_estudante(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, Posicao* caminho, int* passos, int** visitados) {
#if MODO_ANALISE == 1
    chamadas_recursivas++;
    if (nivel > nivel_maximo_recursao) nivel_maximo_recursao = nivel;
#endif
    // Caso base: chegou na primeira linha
    if (x == 0) {
        caminho[*passos].x = x;
        caminho[*passos].y = y;
        (*passos)++;
        labirinto[x][y]=9;
        return 1;
    }

    // Verifica se já visitou a célula
    if (visitados[x][y]) return 0;

    // Marca a posição como visitada
    visitados[x][y] = 1;

    // Adiciona a posição atual ao caminho (mesmo que seja uma tentativa que envolva uma volta)
    caminho[*passos].x = x;
    caminho[*passos].y = y;
    (*passos)++;

    // Movimentos possíveis (cima, esquerda, direita, baixo)
    int movimentos[4][2] = {{-1, 0}, {0, -1}, {0, 1},  {1, 0}};
    
    for (int i = 0; i < 4; i++) {
        int novoX = x + movimentos[i][0];
        int novoY = y + movimentos[i][1];

        if (posicaoValida(labirinto, linhas, colunas, novoX, novoY, chaves)) {
            // Se for uma porta, consome uma chave
            int novasChaves = chaves;
            if (labirinto[novoX][novoY] == 3) {
                novasChaves--;
            }
            // O estudante só usa a chave se passar por uma célula amarela (número 4)
            else if (labirinto[novoX][novoY] == 4) {
                novasChaves++;
            }

            // Verifica se o número de chaves não ficou negativo
            if (novasChaves >= 0) {
                // Tenta mover para a nova posição
                if (movimenta_estudante(labirinto, linhas, colunas, novoX, novoY, novasChaves, nivel + 1, caminho, passos, visitados)) {
                    labirinto[x][y]=9;
                    return 1;
                }
            }
        }
    }

    // Desmarca a posição como visitada antes de retornar
    visitados[x][y] = 0;
    return 0;  // Se não encontrou solução, retorna 0
}
