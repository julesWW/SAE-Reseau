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
		if(memcmp(&reseau->equipement[i],&e,sizeof(Equipement))==0){
			return i;
		}
	}
    return UNKNOWN_INDEX;
}

bool existe_liaison(Reseau_Local *reseau, Liaison l)
{
	for(size_t i=0;i<reseau->nb_liaisons;i++){
		if(reseau->nb_liaisons[i].e1 == l.e1){
			if(reseau->nb_liaisons[i].e2 == l.e2){
				return true;
			}
		}
		else if(reseau->nb_liaisons[i].e1 == l.e2){
			if(reseau->nb_liaisons[i].e2 == l.e1){
				return true;
			}
		}
	}
	return false;
}

size_t index_liaison(Reseau_Local *reseau, Liaison l)
{
	if(!existe_liaison(reseau,l)){
		return UNKNOWN_INDEX;
	}
	for(size_t i=0;i<reseau->nb_liaisons;i++){
		if(reseau->nb_liaisons[i].e1 == l.e1){
			if(reseau->nb_liaisons[i].e2 == l.e2){
				return i;
			}
		}
		else if(reseau->nb_liaisons[i].e1 == l.e2){
			if(reseau->nb_liaisons[i].e2 == l.e1){
				return i;
			}
		}
	}
    return UNKNOWN_INDEX;
}

size_t equipement_adjacents(Reseau_Local *reseau, Equipement e, size_t ea[])
{
	int nb_ea=0;
	for(size_t i=0;i<reseau->nb_liaisons;i++){
		if(reseau.liaisons[i].e1 == index_Equipement(reseau,e)){
			ea[nb_ea]=reseau->liaisons[i].e2;
			nb_ea++;
		}
		else if(reseau->liaisons[i].e2 == index_Equipement(reseau,e)){
			ea[nb_ea]=reseau->liaisons[i].e1;
			nb_ea++;
		}
	}
	return nb_ea;
}
