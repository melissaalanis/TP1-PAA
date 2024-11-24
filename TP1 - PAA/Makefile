#all: compile

run: compile
	./run

compile: Labirinto.o main.o
	gcc -o run Labirinto.o main.o

Labirinto.o: Labirinto.c
	gcc -c Labirinto.c

main.o: main.c
	gcc -c main.c

# Limpar arquivos compilados
clean:
	rm -f run *.o