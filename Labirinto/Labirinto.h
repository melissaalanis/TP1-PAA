#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

// Cores para o terminal
#define cor_vermelha  "\x1b[41m" 
#define cor_azul  "\x1b[46m" 
#define cor_preta "\x1b[100m\x1b[97m" 
#define cor_verde "\x1b[42m" 
#define cor_amarela  "\x1b[43m" 
#define cor_branca  "\x1b[47m"
#define resetar_cor "\x1b[0m"


#define MODO_ANALISE 1 // Trocar para 1 caso queira o modo análise

typedef struct {
    int x;
    int y;
} Posicao;

int** alocaLabirinto(int linhas, int colunas);
void liberaLabirinto(int** labirinto, int linhas);
int posicaoValida(int** labirinto, int linhas, int colunas, int x, int y, int chaves);
int movimentaEstudante(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, Posicao* caminho, unsigned int* passos, int* tamanhoCaminho);
int movimentaEstudanteMatriz(int** labirinto, int linhas, int colunas, int x, int y, int chaves, int nivel, unsigned int* passos);
int processaLabirinto(char nome_arquivo[], int* linhas, int* colunas, int* chaves, Posicao* inicio, int*** labirinto);
void imprimeCaminho(int** labirinto, int linhas, int colunas);
#endif // LABIRINTO_H
