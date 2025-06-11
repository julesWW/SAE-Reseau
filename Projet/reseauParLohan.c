#include "reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

//Fonctions d'initialisation et de deinitialisation


void init_station(Station *station) {
    init_MACAddress(&station->mac); //A Remplacer par juste les valeur dans le fichier config
    init_IPAddrV4(&station->ip) ; //A Remplacer par juste les valeur dans le fichier config
}

//A Remplicer par juste les valeur dans le fichier config
void deinit_station(Station *station) {
	deinit_MacAddress(&station->mac);
	deinit_IPAddrV4(&station->ip);
}

void init_table_comm(TableComm *table) {
    init_MACAddress(table->mac);
    table->port = 0;
}

void deinit_table_comm(TableComm *table) {
	deinit_MACAddress(table->mac);
	table->port = 0;
}

void init_switch(Switch *sw) {
    init_MACAddress(sw->mac);
    sw->nb_ports = 0;
    sw->priorite = 0;
    sw->table_commutation = malloc(INITIAL_CAPACITY * sizeof(TableComm)); // allocation d'un tableau de TableComm de capacité initiale de 8
    if (sw->table_commutation == NULL) {
        fprintf(stderr, "Erreur malloc pour table de commutation.\n");
        exit(EXIT_FAILURE);
    }
}

void deinit_switch(Switch *sw) {
	deinit_MACAddress(sw->mac);
	sw->nb_ports = 0;
    sw->priorite = 0;
    free(sw->table_commutation);
    sw->table_commutation=NULL;
}

void init_reseau_local(Reseau_Local *reseau) {
    reseau->nb_equipements = 0;
    reseau->equipement_capacite=8;
    reseau->equipement = malloc(INITIAL_CAPACITY * sizeof(Equipement));// allocation d'un tableau d'equipement de capacité initiale de 8
    reseau->nb_liaisons = 0;
    reseau->liaison_capacite=8;
    reseau->liaisons = malloc(INITIAL_CAPACITY * sizeof(Liaison));// allocation d'un tableau de liaisons de capacité initiale de 8
}

void deinit_reseau_local(Reseau_Local *reseau) {
    reseau->nb_equipements = 0;
    // Il faut probablement ajouter une boucle qui deinit chaque equipement ci-dessous et les TableComm dans les switchs
    free(reseau->equipement);
    reseau->equipement=NULL;
    reseau->nb_liaisons = 0;
    free(reseau->liaisons);
    reseau->liaisons = NULL;
    reseau->liaison_capacite=0;
    reseau->equipement_capacite=0;
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

//Ajout de station / switch
void ajouter_Station(Reseau_Local *reseau,MACAddress *mac,IPAddrV4 *ip){
	Station st;
	init_station(st);
	st->mac=mac;
	st->ip=ip;
	reseau->equipement[reseau->nb_equipements]->valeur->st=st;
	reseau->equipement[reseau->nb_equipements]->type=STATION;
	reseau->nb_equipements++;
}

void ajouter_Switch(Reseau_Local *reseau,MACAddress *mac,size_t nb_ports,size_t priorite){
	Switch sw;
	init_switch(sw);
	sw->mac=mac;
	sw->nb_ports=nb_ports;
	sw->priorite=priorite;
	reseau->equipement[reseau->nb_equipements]->valeur->sw=sw;
	reseau->equipement[reseau->nb_equipements]->type=SWITCH;
	reseau->nb_equipements++;
}

//Chargement du réseau à partir du fichier de configuration
int charger_Reseau(Reseau_Local *reseau) {
	//on suppose que  le reseau est déjà initialisé
    size_t nb_equipements;
    size_t nb_liaisons;

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
        //Je pars du principe que le fichier de configuration est au bon format mais on pourra mettre une vérification au cas où
        if(nligne == 0){
            sscanf(ligne, "%zu %zu", &nb_equipements, &nb_liaisons);
            printf("Nombre d'équipements : %zu, Nombre de liaisons : %zu\n", nb_equipements, nb_liaisons);
        }
        else if(nligne <= nb_equipements){

			//vérif place dans le tableau d'equipement
			if(reseau->nb_equipements==reseau->equipement_capacite){
				reseau->equipement_capacite*=2;
				Equipement* Tab_Temp = malloc(sizeof(Equipement)*reseau->equipement_capacite);
				for(size_t i=0;i<reseau->nb_equipements;i++){
					Tab_Temp[i]=reseau->equipement[i];
				}
				free(reseau->equipement);
				reseau->equipement=Tab_Temp;
			}
            if(ligne[0] == '1'){
                //C'est une station
                //Récupération de l'adresse MAC
                unsigned char mac_octets[6];
                sscanf(ligne + 2, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &mac_octets[0], &mac_octets[1], &mac_octets[2], &mac_octets[3], &mac_octets[4], &mac_octets[5]);
                MACAddress mac_station;
                init_MacAddress(&mac_station);
                mac_station->octets=mac_octets;
                //Récupération de l'adresse IP
                unsigned char ip_octets[4];
                sscanf(ligne + 20, "%hhu.%hhu.%hhu.%hhu", &ip_octets[0], &ip_octets[1], &ip_octets[2], &ip_octets[3]);
                IPAddrV4 ip_station;
                init_IPAddrV4(&ip_station);
                ip_station->octets=ip_octets;
                ajouter_Station(&reseau, &mac_station, &ip_station)
            }
            if(ligne[0] == '2'){
                //C'est un switch
                //Récupération de l'adresse MAC
                unsigned char mac_octets[6];
                sscanf(ligne + 2, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &mac_octets[0], &mac_octets[1], &mac_octets[2], &mac_octets[3], &mac_octets[4], &mac_octets[5]);
                MACAddress mac_switch;
                init_MacAddress(&mac_switch);
                mac_switch->octets=mac_octets;
                //Récupération du nombre de ports
                size_t nb_ports;
                sscanf(ligne + 20, "%zu", &nb_ports);
                //Récupération de la priorité
                size_t priorite;
                sscanf(ligne + 22, "%zu", &priorite);
                //Initialisation du switch
                ajouter_Switch(&reseau,&mac_switch,nb_ports,priorite);
            }
        }
        else if(nligne <= nb_equipements + nb_liaisons){
            //Pas encore fait
        }

    }
    fclose(fconfig);
    //Initialisation du réseau
    return 0;
}
