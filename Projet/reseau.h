#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "m23/graphe.h"



//Adresse MAC
typedef struct MACAddress{
    unsigned char octets[6];
} MACAddress;

//Adresse IP
typedef struct IPAddrV4{

    /*
    char octet1[4];
    char octet2[4];
    char octet3[4];
    char octet4[4];
    */
    unsigned char octets[4];

} IPAddressV4;

//Station : adresse MAC + adresse IP
typedef struct Station{
    MACAddress mac;
    IPAddressV4 ip;
} Station;

//Entrée dans une table de commutation
typedef struct TableComm{
    MACAddress mac;
    unsigned int port;
} TableComm;

//Switch : adresse MAC + nombre de ports + priorité + table de commutation
typedef struct Switch{
    MACAddress mac;
    int nb_ports;
    int priorite;
    int nb_entrees;
    TableComm* table_commutation;
} Switch;

//Réseau local : contient un ensemble de stations et de switches
typedef struct Reseau_Local{
    int nb_stations;
    Station* station;
    int nb_switches;
    Switch* switches;
} Reseau_Local;

typedef struct TrameEthernet
{
    char preambule[7];
    char sfd;
    MACAddress dest[6];
    MACAddress src[6];
    char type[7];
    char data[1500];
    char fcs[4];
} TrameEthernet;

//fonction d'affichage
void afficher_mac(MACAddress *mac);
void afficher_ip(IPAddressV4 *ip);
void afficher_station(Station *station);
void afficher_switch(Switch *sw);
void afficher_reseau(Reseau_Local *reseau);