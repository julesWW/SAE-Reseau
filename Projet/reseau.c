#include "reseau.h"
#include "m23/algos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Fonctions d'initialisation
void init_MacAddress(MACAddress *mac, unsigned char octets[6]) {
    memcpy(mac->octets, octets, 6);
}

void init_IPAddrV4(IPAddrV4 *ip, unsigned char octets[4]) {
    memcpy(ip->octets, octets, 4);
}

void init_station(Station *station, MACAddress *mac, IPAddrV4 *ip) {
    station->mac = *mac;
    station->ip = *ip;
}

void init_table_comm(TableComm *table, MACAddress *mac, unsigned int port) {
    table->mac = *mac;
    table->port = port;
}

void init_switch(Switch *sw, MACAddress *mac, size_t nb_ports, size_t priorite, size_t nb_entrees) {
    sw->mac = *mac;
    sw->nb_ports = nb_ports;
    sw->priorite = priorite;
    sw->nb_entrees = nb_entrees;
    sw->table_commutation = malloc(nb_entrees * sizeof(TableComm));
    if (sw->table_commutation == NULL) {
        fprintf(stderr, "Erreur malloc pour table de commutation.\n");
        exit(EXIT_FAILURE);
    }
}

void init_reseau_local(Reseau_Local *reseau, size_t nb_stations, Station *stations, size_t nb_switchs, Switch *switchs) {
    reseau->nb_stations = nb_stations;
    reseau->station = stations;
    reseau->nb_switchs = nb_switchs;
    reseau->switchs = switchs;
}

void init_TrameEthernet(TrameEthernet *trame, char preambule[7], char sfd, MACAddress *dest, MACAddress *src, char type[7], char *donnees) {
    memcpy(trame->preambule, preambule, 7); //memcpy pour assurer la bonne copie et le \0 à la fin (je crois)
    trame->sfd = sfd;
    //trame->dest = *dest; (marche pas car c'est un tableau)
    memcpy(trame->dest, dest, sizeof(MACAddress)); 
    //trame->src = *src;
    memcpy(trame->src, src, sizeof(MACAddress)); 
    //trame->type = type;
    memcpy(trame->type, &type, sizeof(size_t)); 
    strcpy(trame->donnees, donnees); 
    //Pour le bourrage jsp comment calculer la taille 
    //Pour le fcs non plus -_-
}


//Fonctions d'affichage
void afficher_mac(MACAddress *mac){
    printf("MAC : %02X:%02X:%02X:%02X:%02X:%02X\n",
           mac->octets[0], mac->octets[1], mac->octets[2],
           mac->octets[3], mac->octets[4], mac->octets[5]);
}

void afficher_ip(IPAddrV4 *ip){
    printf(" IP : %u.%u.%u.%u\n",ip->octets[0],ip->octets[1],ip->octets[2],ip->octets[3]);
}

void afficher_station(Station *station){
    afficher_mac(&station->mac);
    afficher_ip(&station->ip);
}

void afficher_switch(Switch *sw) {
    printf("Switch - ");
    afficher_mac(&sw->mac);
    printf("Ports : %d | Priorité : %d\n", sw->nb_ports, sw->priorite);
    printf("Table de commutation (%d entrées) :\n", sw->nb_entrees);
    for (int i = 0; i < sw->nb_entrees; i++) {
        afficher_mac(&sw->table_commutation[i].mac);
        printf(" -> Port %u\n", sw->table_commutation[i].port);
    }
}