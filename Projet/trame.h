#pragma once

#include "reseau.h"
#include <string.h>
#include "graphe.h"

typedef struct TrameEthernet
{
    char preambule[7];
    char sfd;
    MACAddress dest;
    MACAddress src;
    char type[2];
    char donnees[1500];
    uint8_t bourrage[46];
    char fcs[4];
} TrameEthernet;

//initialisation de trame
void creer_trame(TrameEthernet *trame,MACAddress *src, MACAddress *dest, char* donnees);
void init_trame(TrameEthernet *trame);
void deinit_trame(TrameEthernet *trame);
void envoyer_trame(Reseau_Local *reseau, MACAddress *src, MACAddress *dest, char* donnees);
void afficher_trame_utilisateur(TrameEthernet *trame);
size_t transmettre_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t actuel_index,size_t preced_index) ;
size_t station_receptionne_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t reception_index);
size_t mac_en_indice(Reseau_Local *reseau, MACAddress *mac);
size_t switch_receptionne_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t preced_index,Switch *sw, size_t actuel_index);
