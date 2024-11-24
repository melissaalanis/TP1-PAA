#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#define MODO_ANALISE

typedef struct {
    int x;
    int y;
} Posicao;

int** alocaLabirinto(int linhas, int colunas);
void liberaLabirinto(int** labirinto, int linhas);
int posicaoValida(int** labirinto, int linhas, int colunas, int x, int y, int chaves);
int movimenta_estudante(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, Posicao* caminho, int* passos);
void processaLabirinto(char nome_arquivo[], int* linhas, int* colunas, int* chaves, Posicao* inicio, int*** labirinto);
void imprimeLabirinto(int** labirinto, int linhas, int colunas);

#endif // LABIRINTO_H
