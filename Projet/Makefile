executable: reseau.o algos.o graphe.o main.c
	gcc -o executable reseau.o algos.o graphe.o main.c

reseau.o: reseauParLohan.c reseau.h
	gcc -c reseau.c -o reseau.o

algos.o: m23/algos.c m23/algos.h graphe.o
	gcc -c m23/algos.c -o algos.o

graphe.o: m23/graphe.c m23/graphe.h
	gcc -c m23/graphe.c -o graphe.o
