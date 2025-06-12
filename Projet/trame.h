#pragma once

#include "reseau.h"

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
void init_trame(TrameEthernet *trame);
void deinit_trame(TrameEthernet *trame);
void envoyer_trame(Reseau_Local *reseau, MACAddress *src, MACAddress *dest, char* donnees);
void afficher_trame_utilisateur(TrameEthernet *trame);
void transmettre_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t source_index);

