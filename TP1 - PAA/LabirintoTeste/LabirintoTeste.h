#ifndef LABIRINTOTESTE_H
#define LABIRINTOTESTE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int numeroAleatorio(int minimo, int maximo);
int calculaCelulasLivres(int linhas, int colunas, int dificuldade);
int verificaLimites(int linhas, int colunas, int portas, int chaves_caminho, int portal, int dificuldade, int chaves);
int geraLabirintoTeste(int linhas, int colunas, int chaves, int portas, int chaves_caminho, const char* nome_arquivo, int dificuldade, int portal);

#endif 