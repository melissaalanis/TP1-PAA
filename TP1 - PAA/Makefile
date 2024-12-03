#all: compile

run: compile
	./run

compile: Labirinto/Labirinto.o Labirinto/Labirintoteste.o main.o
	gcc -o run Labirinto/Labirinto.o Labirinto/Labirintoteste.o main.o

Labirinto/Labirinto.o: Labirinto/Labirinto.c
	gcc -c Labirinto/Labirinto.c -o Labirinto/Labirinto.o

Labirinto/Labirintoteste.o: Labirinto/Labirintoteste.c
	gcc -c Labirinto/Labirintoteste.c -o Labirinto/Labirintoteste.o

main.o: main.c
	gcc -c main.c

# Limpar arquivos compilados
clean:
	rm -f run Labirinto/*.o *.o