#include "graphe.h"

#define INITIAL_CAPACITY 8
void init_graphe(graphe *g)
{
    // initialise les champs internes du graphe g
    // - allocation d'un tableau d'arêtes de capacité initiale 8
    // - le graphe ne contient initialement ni sommet ni arête
	g->ordre=0;
	g->nb_aretes=0;
	g->aretes_capacite=INITIAL_CAPACITY;
	g->aretes= malloc(sizeof(arete) * g->aretes_capacite);
}

void deinit_graphe(graphe *g)
{
	g->ordre=0;
	g->nb_aretes=0;
	g->aretes_capacite=0;
	free(g->aretes);
	g->aretes=NULL;
    // libère la mémoire qui avait été allouée dans la fonction init_graphe
    // réinitialise les champs internes du graphe g
}

size_t ordre(graphe const *g)
{
    return g->ordre;
}

size_t nb_aretes(graphe const *g)
{
    return g->nb_aretes;
}

void ajouter_sommet(graphe *g)
{
	g->ordre++;
}

size_t index_sommet(graphe const *g, sommet s)
{
	if(s<(g->ordre)){
		return s;
	}
    // retourne l'index du sommet s dans le graphe g
    // la valeur UNKNOWN_INDEX si le sommet n'existe pas dans g

    return UNKNOWN_INDEX;
}

// une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

bool existe_arete(graphe const *g, arete a)
{
	for(size_t i=0;i<g->nb_aretes;i++){
		if(g->aretes[i].s1 == a.s1){
			if(g->aretes[i].s2 == a.s2){
				return true;
			}
		}
		else if(g->aretes[i].s2 == a.s2){
			if(g->aretes[i].s1 == a.s1){
				return true;
			}
		}
		else if(g->aretes[i].s1 == a.s2){
			if(g->aretes[i].s2 == a.s1){
				return true;
			}
		}
		else if(g->aretes[i].s2 == a.s1){
			if(g->aretes[i].s1 == a.s2){
				return true;
			}
		}
	}
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes

    return false;
}

bool ajouter_arete(graphe *g, arete a)
{
    // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :
    //  - les sommets s1 et s2 de a existent dans g
    //  - les sommets s1 et s2 de a sont distincts
    //  - l'arête a n'existe pas dans g

    // /!\ si la capacité actuelle du tableau d'arêtes n'est pas suffisante,
    // /!\ il faut le réallouer.
    // /!\ on peut par exemple doubler la capacité du tableau actuel.

    // retourne true si l'arête a bien été ajoutée, false sinon

	if((a.s1)>=(g->ordre) || (a.s2)>=(g->ordre) ){
		return false;
	}
	if((a.s1)==(a.s2)){
		return false;
	}
	if (existe_arete(g,a)){
		return false;
	}
	if(g->nb_aretes==g->aretes_capacite){
		g->aretes_capacite*=2;
		arete* Tab =  malloc(sizeof(arete)*g->aretes_capacite);
		for(size_t i=0;i<g->nb_aretes;i++){
			Tab[i]=g->aretes[i];
		}
		free(g->aretes);
		g->aretes=Tab;
	}
	g->aretes[g->nb_aretes]=a;
	g->nb_aretes++;
    return true;
}

size_t index_arete(graphe const *g, arete a)
{
	if(!existe_arete(g,a)){
		return UNKNOWN_INDEX;
	}
	for(size_t i=0;i<g->nb_aretes;i++){
		if(g->aretes[i].s1 == a.s1){
			if(g->aretes[i].s2 == a.s2){
				return i;
			}
		}
		else if(g->aretes[i].s2 == a.s2){
			if(g->aretes[i].s1 == a.s1){
				return i;
			}
		}
		else if(g->aretes[i].s1 == a.s2){
			if(g->aretes[i].s2 == a.s1){
				return i;
			}
		}
		else if(g->aretes[i].s2 == a.s1){
			if(g->aretes[i].s1 == a.s2){
				return i;
			}
		}
	}
    // retourne l'index de l'arête au sein du tableau d'arêtes de g si l'arête a existe dans g,
    // la valeur UNKNOWN_INDEX sinon

    return UNKNOWN_INDEX;
}

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[])
{
	int nb_sa=0;
	for(size_t i=0;i<g->nb_aretes;i++){
		if(g->aretes[i].s1 == s){
			sa[nb_sa]=g->aretes[i].s2;
			nb_sa++;
		}
		if(g->aretes[i].s2 == s){
			sa[nb_sa]=g->aretes[i].s1;
			nb_sa++;
		}
	}
	return nb_sa;
    // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
    // (on suppose que s fait bien partie du graphe g)
    // (le tableau sa est supposé assez grand pour contenir les sommets adjacents de s)
}
