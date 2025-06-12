executable: reseau.o algos.o graphe.o main.o
	gcc -o executable reseau.o algos.o graphe.o main.o

reseau.o: reseauParLohan.c reseau.h
	gcc -c reseauParLohan.c -o reseau.o

algos.o: ../m23/algos.c ../m23/algos.h ../m23/graphe.h
	gcc -c ../m23/algos.c -o algos.o

graphe.o: ../m23/graphe.c ../m23/graphe.h
	gcc -c ../m23/graphe.c -o graphe.o

trame.o: trame.c trame.h
	gcc -c trame.c -o trame.o

main.o: main.c reseau.h ../m23/algos.h ../m23/graphe.h
	gcc -c main.c -o main.o

clean:
	rm -f *.o executable