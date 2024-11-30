import matplotlib.pyplot as plt

# Ler os dados do arquivo
tamanhos = []
tempos = []
with open("Grafico/dados_grafico_coluna.txt", "r") as file:
    for line in file:
        tamanho, tempo = line.split()
        tamanhos.append(int(tamanho))
        tempos.append(float(tempo))

# Plotar o gráfico
plt.plot(tamanhos, tempos, marker='o')
plt.xlabel('Tamanho do Labirinto (colunas)')
plt.ylabel('Tempo de Execução (segundos)')
plt.title('Gráfico de Complexidade x Tempo (colunas)')
plt.grid(True)
plt.show()
