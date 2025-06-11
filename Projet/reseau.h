#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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

} IPAddrV4;


//Station : adresse MAC + adresse IP
typedef struct Station{
    MACAddress mac;
    IPAddrV4 ip;
} Station;

//Entrée dans une table de commutation
typedef struct TableComm{
    MACAddress mac;
    unsigned int port;
} TableComm;

//Switch : adresse MAC + nombre de ports + priorité + table de commutation
typedef struct Switch{
    MACAddress mac;
    size_t nb_ports;
    size_t priorite;
    TableComm* table_commutation;
} Switch;

//Valeurs pour différencier station et switch
typedef enum {
    STATION,
    SWITCH
} Type_equipement;

//union pouvant contenir un switch ou une station
typedef union {
    Station st;
    Switch sw;
} Appareil;

//structure contenant l'union Equipement et l'enum Type_equipement pour reconnaitre ce dernier
typedef struct{
	Type_equipement type;
	Appareil valeur;
}Equipement;

//liaison : contient 2 equipement et représente qu'ils sont liés
typedef struct Liaison{
	Equipement e1;
	Equipement e2;
	size_t poids;
}Liaison;

//Réseau local : contient un ensemble de stations et de switches
typedef struct Reseau_Local{
	size_t equipement_capacite;
    size_t nb_equipements;
    Equipement* equipement;
    size_t liaison_capacite;
    size_t nb_liaisons;
    Liaison* liaisons;
} Reseau_Local;

typedef struct TrameEthernet
{
    char preambule[7];
    char sfd;
    MACAddress dest;
    MACAddress src;
    char type[7];
    char donnees[1500];
    uint8_t bourrage[46];
    char fcs[4];
} TrameEthernet;

//Fonctions d'initialisation
void init_IPAddrV4(IPAddrV4 *ip, unsigned char octets[4]);

void init_MacAddress(MACAddress *mac);
//void init_MACAddress(MACAddress *mac, unsigned char octets[6]);


void init_station(Station *station, MACAddress *mac, IPAddrV4 *ip);
void init_table_comm(TableComm *table, MACAddress *mac, unsigned int port);
void init_switch(Switch *sw, MACAddress *mac, size_t nb_ports, size_t priorite, size_t nb_entrees);
void init_TrameEthernet(TrameEthernet *trame, char preambule[7], char sfd, MACAddress *dest, MACAddress *src, char type[7], char *donnees);
void init_reseau_local(Reseau_Local *reseau, size_t nb_stations, Station *stations, size_t nb_switches, Switch *switches);

//Fonctions d'affichage
void afficher_mac(MACAddress *mac);
void afficher_ip(IPAddrV4 *ip);
void afficher_station(Station *station);
void afficher_switch(Switch *sw);
void afficher_reseau(Reseau_Local *reseau);
