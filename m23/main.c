#include "graphe.h"
#include "algos.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    graphe g; // déclaration
init_graphe(&g);
for (size_t i = 0; i < 5; ++i)
ajouter_sommet(&g);
printf("ordre = %zu\n", ordre(&g));
deinit_graphe(&g);

    return EXIT_SUCCESS;
}
