#include "algos.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

size_t degre(graphe const *g, sommet s)
{
	size_t* Tab =	 malloc(sizeof(size_t) * g->nb_aretes);
    return sommets_adjacents(g,s,Tab);
}

bool est_regulier(graphe const *g)
{
    return false;
}

void afficher(graphe const *g)
{
}

void generer_complet(graphe *g, size_t ordre)
{
}

void visite_composante_connexe(graphe const *g, sommet s, bool *visite)
{
}

uint32_t nb_composantes_connexes(graphe const *g)
{
    return 0;
}

bool sont_connectes(graphe const *g, sommet s1, sommet s2)
{
    return false;
}

void coloriage_glouton(graphe const *g, uint8_t *couleur_sommet)
{
}

void appliquer_permutation(graphe const *src, graphe *dst, size_t const *permutation)
{
}

uint32_t estimation_nb_chromatique(graphe const *g, uint32_t n)
{
    return 0;
}

void generer_aleatoire(graphe *g, size_t ordre, uint32_t k)
{
}

void dijkstra(graphe const *g, sommet s, double const *poids_arete, double *distance_sommet)
{
}

void trier_aretes(arete *aretes_triees, graphe const *g, double const *poids_arete)
{
}

void kruskal(graphe const *g, double const *poids_arete, graphe *acm)
{
}
