executable: reseau.o graphe.o main.o trame.o
	gcc -o executable reseau.o graphe.o main.o trame.o

reseau.o: reseau.c reseau.h
	gcc -c reseau.c -o reseau.o


graphe.o: graphe.c graphe.h
	gcc -c graphe.c -o graphe.o

trame.o: trame.c trame.h
	gcc -c trame.c -o trame.o

main.o: main.c reseau.h graphe.h
	gcc -c main.c -o main.o

clean:
	rm -f *.o executable
