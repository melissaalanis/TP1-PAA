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
void imprimeCaminho(int** labirinto, int linhas, int colunas){
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if(labirinto[i][j]==9){
                printf(cor_verde "  " resetar_cor);
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
        if (labirinto[x][y] == 1 || labirinto[x][y] == 4 || labirinto[x][y] == 5 || labirinto[x][y] == 6) { // Celula livre ou chave
            return 1;
        } else if (labirinto[x][y] == 3 && chaves > 0) { // Porta e tem chave
            return 1;
        }
    }
    return 0;
}

// Funcao recursiva para movimentar o estudante
int movimenta_estudante(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, Posicao* caminho, unsigned int* passos, int* tamanho_caminho) {
    #if MODO_ANALISE == 1 /// Variaveis para analise
        chamadas_recursivas++;
        if (nivel > nivel_maximo_recursao) nivel_maximo_recursao = nivel; 
    #endif

    // Caso base: chegou na primeira linha
    if (x == 0) {
        labirinto[x][y] = 9;
        caminho[*tamanho_caminho].x = x;
        caminho[*tamanho_caminho].y = y;
        (*tamanho_caminho)++;
        (*passos)++;
        return 1;
    }

    // Marca a posicao como visitada
    int estado_atual = labirinto[x][y];
    labirinto[x][y] = 9;

    if(estado_atual == 5){
        // Busca a saída do portal (valor 6 no labirinto)
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (labirinto[i][j] == 6) {
                    // Tenta mover para a nova posicao
                    if (movimenta_estudante(labirinto, linhas, colunas, i, j, chaves, nivel + 1, caminho, passos, tamanho_caminho)) {
                        // Se conseguiu, adiciona a posicao ao caminho e atuliza seu tamanho
                        caminho[*tamanho_caminho].x = x;
                        caminho[*tamanho_caminho].y = y;
                        
                        (*tamanho_caminho)++;
                        return 1;
                    }

                }
            }
        }
    }

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
            int novas_chaves = chaves;
            if (labirinto[novoX][novoY] == 3) {
                novas_chaves--;
            }

            // O estudante encontrou uma chave
            else if (labirinto[novoX][novoY] == 4) {
                novas_chaves++;
            }

            // Tenta mover para a nova posicao
            if (movimenta_estudante(labirinto, linhas, colunas, novoX, novoY, novas_chaves, nivel + 1, caminho, passos, tamanho_caminho)) {
                // Se conseguiu, adiciona a posicao ao caminho e atuliza seu tamanho
                caminho[*tamanho_caminho].x = x;
                caminho[*tamanho_caminho].y = y;
                //*matriz[x][y] = 9;
                (*tamanho_caminho)++;
                return 1;
            }
        }
    }

    // Desmarca a posicao e retorna ao estado anterior
    labirinto[x][y] = estado_atual;
    return 0;
}

// Imprime cada movimento do estudante no labirinto (desenho colorido)
int movimentaEstudanteMatriz(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, unsigned int* passos) {
    #if MODO_ANALISE == 1
        chamadas_recursivas++;
        if (nivel > nivel_maximo_recursao) nivel_maximo_recursao = nivel;
    #endif

    // Caso base: chegou na primeira linha
    if (x == 0) {
        labirinto[x][y] = 9; // Marca a posicao como visitada
        printf("Linha: %d Coluna: %d\n", x, y);
        imprimeCaminho(labirinto, linhas, colunas);  // Mostra o labirinto atualizado
        printf("\n");

        (*passos)++;
        printf("\nO estudante se movimentou %d vezes e chegou na coluna %d da primeira linha\n\n",*passos, y);
        return 1;
    }

    // Marca a posicao como visitada
    int estado_atual = labirinto[x][y];
    labirinto[x][y] = 9;  // Marca o caminho atual com 9 (verde)
    printf("Linha: %d Coluna: %d\n", x, y);
    imprimeCaminho(labirinto, linhas, colunas);  // Atualiza a visualização do labirinto
    printf("\n");

    if(estado_atual == 5){
        // Busca a saída do portal (valor 6 no labirinto)
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                if (labirinto[i][j] == 6) {
                    
                    printf("Transportando para a saída do portal (%d, %d).\n", i, j);
                    // Tenta mover para a nova posicao
                    printf("\n");
                    if (movimentaEstudanteMatriz(labirinto, linhas, colunas, i, j, chaves, nivel + 1, passos)) {
                        return 1;
                    }

                }
            }
        }
    }

    // Movimentos possíveis (cima, esquerda, direita, baixo)
    int movimentos[4][2] = {{-1, 0}, {0, -1}, {0, 1},  {1, 0}};
    
    // Tenta mover para as 4 direções
    for (int i = 0; i < 4; i++) {
        // Calcula a nova posição
        int novoX = x + movimentos[i][0];
        int novoY = y + movimentos[i][1];

        if (posicaoValida(labirinto, linhas, colunas, novoX, novoY, chaves)) {  // Verifica se a posição é válida
            (*passos)++;  // Contabiliza as movimentações do estudante

            // Ajusta as chaves caso encontre uma porta ou chave
            int novas_chaves = chaves;
            if (labirinto[novoX][novoY] == 3) {  // Porta
                novas_chaves--;
            } else if (labirinto[novoX][novoY] == 4) {  // Chave
                novas_chaves++;
            }

            // Move para a nova posição
            if (movimentaEstudanteMatriz(labirinto, linhas, colunas, novoX, novoY, novas_chaves, nivel + 1, passos)) {
                return 1;
            }
        }
    }

    // Backtracking: desmarca a posição e restaura o estado original
    labirinto[x][y] = estado_atual;  // Restaura o valor original
    printf("Linha: %d Coluna: %d\n", x, y);
    imprimeCaminho(labirinto, linhas, colunas);  // Atualiza a visualização durante o backtracking
    printf("\n");


    return 0;  // Indica que não foi possível encontrar um caminho
}
