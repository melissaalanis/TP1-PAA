#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Aloca memoria para o labirinto
int** alocaLabirinto(int linhas, int colunas) {
    int** labirinto = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        labirinto[i] = (int*)malloc(colunas * sizeof(int));
    }
    return labirinto;
}

// Libera a memoria do labirinto
void liberaLabirinto(int** labirinto, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(labirinto[i]);
    }
    free(labirinto);
}

// Gera um número aleatorio entre minimo e maximo
int numeroAleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

// Gera o labirinto de teste com base nos parametros passados
void geraLabirintoTeste(int linhas, int colunas, int chaves, int chaves_caminho, int portas, const char* nomeArquivo) {
    int** labirinto = alocaLabirinto(linhas, colunas);
    FILE* arquivo = fopen(nomeArquivo, "w");

    if (!arquivo) {
        printf("Erro ao criar o arquivo!\n");
        liberaLabirinto(labirinto, linhas);
        return;
    }
    fprintf(arquivo, "%d %d %d\n", linhas, colunas, chaves); // coloca no arq as informacoes

    // Inicializa o labirinto com caminhos livres - numeros 1's (celulas vazias)
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            labirinto[i][j] = 1;  
        }
    }

    // Coloca paredes aleatorias no labirinto - numeros 2's (paredes)
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (numeroAleatorio(0, 2) == 0 && (i != 0 || j != 0)) {  // A cada 3 posicoes, uma tem q ser parede (fora na posição inicial)
                labirinto[i][j] = 2;  // Coloca parede
            }
        }
    }

    // Posicao do estudante eh zero 
    int x = linhas - 1;
    int y = numeroAleatorio(1, colunas - 2);  // Posicao aleatoria na ultima linha, exceto nas extremidades do labirinto
    labirinto[x][y] = 0;  // posicao inicial do estudante

    // Criando caminhos livres partindo da posicao inicial do estudante
    while (x > 0) {
        int direcao = numeroAleatorio(0, 2);  // Cima, esquerda ou direita
        if (direcao == 0 && x > 0) {  
            x--;  // Move para cima
        } else if (direcao == 1 && y > 1) {
            y--;  // Move para a esquerda
        } else if (direcao == 2 && y < colunas - 2) {
            y++;  // Move para a direita
        }

        if (labirinto[x][y] != 0) {  
            labirinto[x][y] = 1;  // Marca como celula vazio
        }
    }

    // Adiciona portas aleatoriamente em células de caminho livre - coloca 3's (portas)
    int qtdPortas = portas;
    while(qtdPortas > 0) {
        int px = numeroAleatorio(1, linhas - 2);  // Dentro dos limites
        int py = numeroAleatorio(1, colunas - 2);  // DECIDIR ISSO DEPOIS
        if (labirinto[px][py] == 2) {  // Porta so em celulas vazias
            labirinto[px][py] = 3;  
            labirinto[px][py + 1] = 2; // DECISAO DA EQUIPE: QUANDO UMA PORTA É GERADA, ELA É GERADA ENTRE PAREDES
            labirinto[px][py-1] = 2;
            qtdPortas--;
        }
    }

    int chaves_adc = 0;
    while (chaves_adc < chaves_caminho) {
        int cx = numeroAleatorio(1, linhas - 1);  
        int cy = numeroAleatorio(1, colunas - 1);  
        if (labirinto[cx][cy] == 1) {  // A chave so pode ser colocada em um caminho livre
            labirinto[cx][cy] = 4;  // Marca como chave
            chaves_adc++;
        }
}

    // Salva o labirinto no arquivo
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "%d", labirinto[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    liberaLabirinto(labirinto, linhas);
}

int main() {
    srand(time(NULL));  // Inicializa o gerador de numeros aleatorios

    int linhas, colunas, chaves, portas, chaves_caminho;
    char nomeArquivo[100];


    linhas = 10;
    colunas = 10;
    chaves = 3;
    portas = 2;
    chaves_caminho = 1;
    
    strcpy(nomeArquivo, "AAAA.txt");

    // Gera o labirinto e salva no arquivo especificado
    geraLabirintoTeste(linhas, colunas, chaves, chaves_caminho, portas, nomeArquivo);

    printf("Labirinto gerado e salvo no arquivo '%s'\n", nomeArquivo);

    return 0;
}
