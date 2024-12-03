#ifndef LABIRINTOTESTE_H
#define LABIRINTOTESTE_H

int** alocaLabirintoTeste(int linhas, int colunas);
void liberaLabirintoTeste(int** labirinto, int linhas);
int numeroAleatorio(int minimo, int maximo);
void geraLabirintoTeste(int linhas, int colunas, int chaves, int chaves_caminho, int portas, const char* nomeArquivo, int dificuldade);


#endif 