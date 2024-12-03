#include "Labirinto.h"

#if MODO_ANALISE == 1 // Variaveis para analise
    int chamadas_recursivas = 0;
    int nivel_maximo_recursao = 0;
#endif

// Funcao para alocar memoria para o labirinto
int** alocaLabirinto(int linhas, int colunas) {
    int** labirinto = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        labirinto[i] = (int*)malloc(colunas * sizeof(int));
    }
    return labirinto;
    
}

// Funcao para preencher o labirinto
int processaLabirinto(char nome_arquivo[], int* linhas, int* colunas, int* chaves, Posicao* inicio, int*** labirinto){
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
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
    return 0;
}

// Imprime o labirinto colorido
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

// Imprime a matriz que sera usada para mostrar o caminho
void imprimeMatriz(int** labirinto, int linhas, int colunas){
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            //printf("%d ", labirinto[i][j]);
            if(labirinto[i][j]==-1){
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

// Funcao para liberar memoria do labirinto
void liberaLabirinto(int** labirinto, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(labirinto[i]);
    }
    free(labirinto);
}

// Funcao para verificar se a posicao eh valida
int posicaoValida(int** labirinto, int linhas, int colunas, int x, int y, int chaves) {
    if (x >= 0 && x < linhas && y >= 0 && y < colunas) { // Dentro dos limites
        if (labirinto[x][y] == 1 || labirinto[x][y] == 4) { // Celula livre ou chave
            return 1;
        } else if (labirinto[x][y] == 3 && chaves > 0) { // Porta e tem chave
            return 1;
        }
    }
    return 0;
}

// Funcao recursiva para movimentar o estudante
int movimenta_estudante(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, Posicao* caminho, unsigned int* passos, int* tamanhoCaminho/*, int*** matriz*/) {
    #if MODO_ANALISE == 1 /// Variaveis para analise
        chamadas_recursivas++;
        if (nivel > nivel_maximo_recursao) nivel_maximo_recursao = nivel; 
    #endif

    // Caso base: chegou na primeira linha
    if (x == 0) {
        labirinto[x][y] = -1;
        caminho[*tamanhoCaminho].x = x;
        caminho[*tamanhoCaminho].y = y;
        (*tamanhoCaminho)++;
        (*passos)++;
        return 1;
    }

    // Marca a posicao como visitada
    int estadoAtual = labirinto[x][y];
    labirinto[x][y] = -1;

    // Movimentos possiveis (cima, esquerda, direita, baixo)
    int movimentos[4][2] = {{-1, 0}, {0, -1}, {0, 1},  {1, 0}};
    
    // Tenta mover para as 4 direcoes
    for (int i = 0; i < 4; i++) {
        // Calcula a nova posicao
        int novoX = x + movimentos[i][0];
        int novoY = y + movimentos[i][1];

        if (posicaoValida(labirinto, linhas, colunas, novoX, novoY, chaves)) { // Vetifica se a posicao eh valida
            (*passos)++; // Contabiliza todas as movimentacoes do estudante

            // Se for uma porta, consome uma chave
            int novasChaves = chaves;
            if (labirinto[novoX][novoY] == 3) {
                novasChaves--;
            }

            // O estudante encontrou uma chave
            else if (labirinto[novoX][novoY] == 4) {
                novasChaves++;
            }

            // Tenta mover para a nova posicao
            if (movimenta_estudante(labirinto, linhas, colunas, novoX, novoY, novasChaves, nivel + 1, caminho, passos, tamanhoCaminho)) {
                // Se conseguiu, adiciona a posicao ao caminho e atuliza seu tamanho
                caminho[*tamanhoCaminho].x = x;
                caminho[*tamanhoCaminho].y = y;
                //*matriz[x][y] = 9;
                (*tamanhoCaminho)++;
                return 1;
            }
        }
    }

    // Desmarca a posicao e retorna ao estado anterior
    labirinto[x][y] = estadoAtual;
    return 0;
}