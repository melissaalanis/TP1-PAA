#include "LabirintoTeste.h"

// Gera um numero aleatorio entre minimo e maximo
int numeroAleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

int calculaCelulasLivres(int linhas, int colunas, int dificuldade) {
    int total_celulas = linhas * colunas;
    /*Como são criadas paredes de acordo com a dificuldade, faz se uma estimativa de quantas 
    serão criadas uma vez que é aleatoriamente e não tem como prever
    A previsão é feita dividindo a quantidade total de células pela quantidade 
    de probabilidade de paredes escolhidas na geração de labirintos*/
    int paredes_estimada = 0;
    if (dificuldade == 1) {
        paredes_estimada = total_celulas / 5; // 20%
    } else if (dificuldade == 2) {
        paredes_estimada = total_celulas / 4; // 25%
    } else if (dificuldade >= 3) {
        paredes_estimada = total_celulas / 3; // 33%
    }
    //Apos estimar a quantidade 
    int reservadas = 1; // Para entrada 
    int celulas_livres = total_celulas - paredes_estimada - reservadas;
    return celulas_livres;  
}

int verificaLimites(int linhas, int colunas, int portas, int chaves_caminho, int portal, int dificuldade, int chaves) {
    // Verifica se os elemntos do labirinto são lógicos
    if(linhas <= 0 || colunas <= 0 || portas < 0 || chaves_caminho < 0 || portal < 0 || dificuldade <= 0 || chaves < 0){ 
        return 1;
    }
    int celulas_livres = calculaCelulasLivres(linhas, colunas, dificuldade);
    // Soma a quantidade de elementos, para o portal, caso exista, é contabilizado 2 pois possui entrada e saída
    int elementos = portas * 3 + chaves_caminho + (portal > 0 ? 2 : 0); 
    //Para as portas, o valor é multiplicado por 3, pois ao se criar uma porte duas paredes são adicionadas ao seu lado
    //Caso a quantidade de elementos seja maior ou igual a quantidade de celulas livres divido por 2 não é possível criar um labirinto  
    if (elementos >= celulas_livres / 2 ) {      
    // A comparação é feita com celulas livres / 2 para seguir a proporção de labirintos pequenos, porque por mais que 
    //tenha celulas livres pode não ser viável e lógico criar o labirinto
        return 1;  
    }
    if(colunas <3){ // Levando em consideração a maneira que que foi implementada a criação de labiritos 
    //se tivesse menos que 3 colunas seria inviavel 
        return 1;
    }
    return 0; 
}

// Gera o labirinto de teste com base nos parametros passados
int geraLabirintoTeste(int linhas, int colunas, int chaves, int portas, int chaves_caminho, const char* nome_arquivo, int dificuldade, int portal) {
    int labirinto[linhas][colunas]; // Cria o labirinto
    
    FILE* arquivo = fopen(nome_arquivo, "w"); // Cria o arquivo para salvar o labirinto
    if (!arquivo) {
        printf("Erro ao criar o arquivo!\n");
        return 1; // Retorna 1 se houver erro
    }

    fprintf(arquivo, "%d %d %d\n", linhas, colunas, chaves); // coloca no arquivo as informacoes

    // Inicializa o labirinto com caminhos livres - numeros 1's (celulas vazias)
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            labirinto[i][j] = 1;  
        }
    }

    if(dificuldade > 3){
        dificuldade = 3;
    }

    // Coloca paredes aleatorias no labirinto - numeros 2's (paredes)
    for (int i = 0; i < linhas; i++) { // A DIFICULDADE TEM A VER COM A GERACAO DE PAREDES ALEATORIAS
        for (int j = 0; j < colunas; j++) {
            if(dificuldade == 1){ // DIFICULDADE FACIL
                if (numeroAleatorio(0, 4) == 0 && (i != 0 || j != 0)) {  // A cada 5 posicoes, uma tem chance ser parede (fora na posição inicial)
                labirinto[i][j] = 2;  // Coloca parede
                }
            }
            else if(dificuldade == 2){ // DIFICULDADE MEDIA
                if (numeroAleatorio(0, 3) == 0 && (i != 0 || j != 0)) {  // A cada 4 posicoes, uma tem chance ser parede (fora na posição inicial)
                labirinto[i][j] = 2;  // Coloca parede
                }
            }
            else if(dificuldade >= 3){ // SE FOR UM NUMERO MAIOR OU IGUAL A 3
                 if (numeroAleatorio(0, 2) == 0 && (i != 0 || j != 0)){  // A cada 3 posicoes, uma tem chance de ser parede (fora na posição inicial)
                    labirinto[i][j] = 2; 
                 }
            }
            else{
                printf("Dificuldade Invalida");
            }
        }
    }

    // Posicao do estudante eh zero 
    int x = linhas - 1;
    int y = numeroAleatorio(1, colunas - 2);  // Posicao aleatoria na ultima linha, exceto nas extremidades do labirinto
    labirinto[x][y] = 0;  // posicao inicial do estudante

    // Adiciona portas aleatoriamente em celulas de caminho livre - coloca 3's (portas)
    
    int qtdPortas = portas;

    while(qtdPortas > 0) {
        int px = numeroAleatorio(1, linhas - 2);  // Dentro dos limites
        int py = numeroAleatorio(1, colunas - 1); 
        if (labirinto[px][py] == 1) {  // Porta so em celulas vazias
            labirinto[px][py] = 3;  // Marca como porta
            labirinto[px][py + 1] = 2; // DECISAO DA EQUIPE: QUANDO UMA PORTA É GERADA, ELA É GERADA ENTRE PAREDES
            labirinto[px][py-1] = 2;
            qtdPortas--;
        }
    }
    
    int qtd_chaves = chaves_caminho;
    int chaves_adc = 0;

    while (chaves_adc < qtd_chaves) {
        int cx = numeroAleatorio(1, linhas - 1);  
        int cy = numeroAleatorio(1, colunas - 1);  
        if (labirinto[cx][cy] == 1) {  // A chave so pode ser colocada em um caminho livre
            labirinto[cx][cy] = 4;  // Marca como chave
            chaves_adc++;
        }
    }  
    
    if(portal > 0){
        while (1){
            int ix = numeroAleatorio(1, linhas - 2);  // Dentro dos limites
            int iy = numeroAleatorio(1, colunas - 1); 
            if(labirinto[ix][iy] == 1 || labirinto[ix][iy] == 2 ){ // Precisa ser uma celula livre ou parede
                labirinto[ix][iy] = 5; // Marca como entrada do portal
                break;
            }
        }
        while (1){
            int fx = numeroAleatorio(1, linhas - 2);  // Dentro dos limites
            int fy = numeroAleatorio(1, colunas - 1); 
            if(labirinto[fx][fy] == 1 || labirinto[fx][fy] == 2 ){ // Precisa ser uma celula livre ou parede
                labirinto[fx][fy] = 6; // Marca como saida do portal
                break;
            }
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
    return 0;
}


