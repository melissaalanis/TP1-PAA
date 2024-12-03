#ifndef LABIRINTOTESTE_H
#define LABIRINTOTESTE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int** alocaLabirintoTeste(int linhas, int colunas);
void liberaLabirintoTeste(int** labirinto, int linhas);
int numeroAleatorio(int minimo, int maximo);
int geraLabirintoTeste(int linhas, int colunas, int chaves, int chaves_caminho, int portas, const char* nomeArquivo, int dificuldade, int portal);

#endif 