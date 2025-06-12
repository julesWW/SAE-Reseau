#include "graphe.h"

size_t ordre(Reseau_Local *reseau)
{
    return reseau->nb_equipements;
}
size_t nb_liaisons(Reseau_Local *reseau)
{
    return reseau->nb_liaisons;
}

size_t index_Equipement(Reseau_Local *reseau, Equipement e)
{
	for(size_t i=0;i<ordre(reseau);i++){
		if(reseau->equipement==e){
			memcmp(reseau->equipement[i],e,sizeof(Equipement)==0){
				return i;
			}
		}
	}
    return -1;
}

bool existe_liaison(Reseau_Local *reseau, Liaison l)
{
	for(size_t i=0;i<reseau->nb_liaisons;i++){
		if(reseau->nb_liaisons[i].s1 == l.e1){
			if(reseau->nb_liaisons[i].s2 == l.e2){
				return true;
			}
		}
		else if(reseau->nb_liaisons[i].s1 == l.e2){
			if(reseau->nb_liaisons[i].s2 == l.e1){
				return true;
			}
		}
	}
	return false;
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
