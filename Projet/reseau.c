#include "reseau.h"
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
    printf("Ports : %ld | Priorité : %ld\n", sw->nb_ports, sw->priorite);
    printf("Table de commutation (%ld entrées) :\n", sw->nb_entrees);
    for (size_t i = 0; i < sw->nb_entrees; i++) {
        afficher_mac(&sw->table_commutation[i].mac);
        printf(" -> Port %u\n", sw->table_commutation[i].port);
    }
}


int initReseau() {

    Reseau_Local reseau;
    size_t nb_equipements;
    size_t nb_liaisons;
    size_t nb_stations = 0;
    size_t nb_switchs = 0;

    //Lecture du fichier de configuration
    FILE *fconfig = fopen("config.txt", "r");
    if (fconfig == NULL) {
        perror("Erreur d'ouverture du fichier de configuration.\n");
        return EXIT_FAILURE;
    }

    char ligne[256];
    size_t nligne = 0;
    while(fgets(ligne, sizeof(ligne), fconfig)) {
        nligne++;
        if(nligne == 0){
            //Je pars du principe que le fichier de configuration est au bon format mais on pourra mettre une vérification au cas où
            sscanf(ligne, "%zu %zu", &nb_equipements, &nb_liaisons);
            printf("Nombre d'équipements : %zu, Nombre de liaisons : %zu\n", nb_equipements, nb_liaisons);
        }
        else if(nligne <= nb_equipements){
            if(ligne[0] == '1'){
                //C'est une station
                nb_stations++;
                char nom_station[50];
                sprintf(nom_station, "station%zu", nb_stations);
                Station nom_station;
                //Récupération de l'adresse MAC
                unsigned char mac_octets[6];
                sscanf(ligne + 2, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &mac_octets[0], &mac_octets[1], &mac_octets[2], &mac_octets[3], &mac_octets[4], &mac_octets[5]);
                MACAddress mac_station;
                init_MacAddress(&mac_station, mac_octets);
                //Récupération de l'adresse IP
                unsigned char ip_octets[4];
                sscanf(ligne + 20, "%hhu.%hhu.%hhu.%hhu", &ip_octets[0], &ip_octets[1], &ip_octets[2], &ip_octets[3]);
                IPAddrV4 ip_station;
                init_IPAddrV4(&ip_station, ip_octets);
                init_station(&nom_station, &mac_station, &ip_station);
            }
            if(ligne[0] == '2'){
                //C'est un switch
                nb_switchs++;
                char nom_switch[50];
                sprintf(nom_switch, "switch%zu", nb_switchs);
                Switch nom_switch;
                //Récupération de l'adresse MAC
                unsigned char mac_octets[6];
                sscanf(ligne + 2, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &mac_octets[0], &mac_octets[1], &mac_octets[2], &mac_octets[3], &mac_octets[4], &mac_octets[5]);
                MACAddress mac_switch;
                init_MacAddress(&mac_switch, mac_octets);
                //Récupération du nombre de ports
                size_t nb_ports;
                sscanf(ligne + 20, "%zu", &nb_ports);
                //Récupération de la priorité
                size_t priorite;
                sscanf(ligne + 22, "%zu", &priorite);
                //Initialisation du switch
                init_switch(&nom_switch, &mac_switch, nb_ports, priorite, nb_ports);
                //Initialisation de la table de commutation
                /*nom_switch.table_commutation = malloc(nb_ports * sizeof(TableComm));
                if (nom_switch.table_commutation == NULL) {
                    fprintf(stderr, "Erreur malloc pour table de commutation.\n");
                    fclose(fconfig);
                    return EXIT_FAILURE;
                }*/
                ///!\jsp comment faire pour ça car il prend nom_switch comme char et non comme switch/!\

            }

        }
        else if(nligne <= nb_equipements + nb_liaisons){
            //Pas encore fait
        }

    }
    fclose(fconfig);
    //Initialisation du réseau
    reseau.nb_stations = nb_stations;
    reseau.station = malloc(nb_stations * sizeof(Station));
    if (reseau.station == NULL) {
        fprintf(stderr, "Erreur malloc pour les stations.\n");
        return EXIT_FAILURE;
    }
    reseau.nb_switchs = nb_switchs;
    reseau.switchs = malloc(nb_switchs * sizeof(Switch));
    if (reseau.switchs == NULL) {
        fprintf(stderr, "Erreur malloc pour les switches.\n");
        free(reseau.station);
        return EXIT_FAILURE;
    }


    return 0;
}
