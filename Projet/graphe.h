#pragma once

#include "reseau.h"

#define UNKNOWN_INDEX ((size_t)-1)

size_t equipement_adjacents(Reseau_Local *reseau, Equipement e, size_t ea[]);
size_t index_liaison(Reseau_Local *reseau, Liaison l);
bool existe_liaison(Reseau_Local *reseau, Liaison l);
size_t index_Equipement(Reseau_Local *reseau, Equipement e);
size_t nb_liaisons(Reseau_Local *reseau);
size_t ordre(Reseau_Local *reseau);
