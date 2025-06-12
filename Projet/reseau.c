#include "reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

//Fonctions d'initialisation et de deinitialisation

void init_IPAddrV4(IPAddrV4 *ip){
    for(size_t i = 0; i < 4; i++){
        ip->octets[i] = 0;
    }
}

void deinit_IPAddrV4(IPAddrV4 *ip){
    for(size_t i = 0; i < 4; i++){
        ip->octets[i] = 0;
    }
}
                                                //Pour ces 4 là je sais pas trop quoi mettre
void init_MacAddress(MACAddress *mac){
    for(size_t i = 0; i < 6; i++){
        mac->octets[i] = 0;
    }
}

void deinit_MacAddress(MACAddress *mac){
    for(size_t i = 0; i < 6; i++){
        mac->octets[i] = 0;
    }
}

void init_station(Station *station){
    init_MacAddress(&station->mac);
    init_IPAddrV4(&station->ip) ;
}
void deinit_station(Station *station){
	deinit_MacAddress(&station->mac);
	deinit_IPAddrV4(&station->ip);
}

void init_table_comm(TableComm *table){
    init_MacAddress(&table->mac);
    table->port = 0;
}

void deinit_table_comm(TableComm *table){
	deinit_MacAddress(&table->mac);
	table->port = 0;
}

void init_switch(Switch *sw){
    init_MacAddress(&sw->mac);
    sw->nb_ports = 0;
    sw->priorite = 0;
    // Allocation d'un tableau de TableComm de capacité initiale de 8
    sw->table_commutation = malloc(INITIAL_CAPACITY * sizeof(TableComm));
    if (sw->table_commutation == NULL) {
        fprintf(stderr, "Erreur malloc pour table de commutation.\n");
        exit(EXIT_FAILURE);
    }
}

void deinit_switch(Switch *sw){
	deinit_MacAddress(&sw->mac);
	sw->nb_ports = 0;
    sw->priorite = 0;
    free(sw->table_commutation);
    sw->table_commutation=NULL;
}

void init_reseau_local(Reseau_Local *reseau){
    reseau->nb_equipements = 0;
    reseau->equipement_capacite=8;
    //Allocation d'un tableau d'equipement de capacité initiale de 8
    reseau->equipement = malloc(INITIAL_CAPACITY * sizeof(Equipement));
    if (reseau->equipement == NULL) {
        fprintf(stderr, "Erreur malloc pour equipement.\n");
        exit(EXIT_FAILURE);
    }
    reseau->nb_liaisons = 0;
    reseau->liaison_capacite=8;
    //Allocation d'un tableau d'equipement de capacité initiale de 8
    reseau->liaisons = malloc(INITIAL_CAPACITY * sizeof(Liaison));
    if (reseau->liaisons == NULL) {
        fprintf(stderr, "Erreur malloc pour liaisons.\n");
        exit(EXIT_FAILURE);
    }
}

void deinit_reseau_local(Reseau_Local *reseau){
    //J'espère que la boucle est bonne
    for (size_t i = 0; i < reseau->nb_equipements; i++) {
        if (reseau->equipement[i].type == SWITCH) {
            free(reseau->equipement[i].valeur.sw.table_commutation);
        }
    }
    reseau->nb_equipements = 0;
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
    printf("Table de commutation :\n");
    for (size_t i = 0; i < sw->nb_ports; i++) {
        afficher_mac(&sw->table_commutation[i].mac);
        printf(" -> Port %u\n", sw->table_commutation[i].port);
    }
}

//Ajout de station / switch
bool ajouter_Station(Reseau_Local *reseau, MACAddress *mac, IPAddrV4 *ip){
	if(reseau->nb_equipements>=reseau->equipement_capacite){
		reseau->equipement_capacite*=2;
		Equipement* Tab_Temp = malloc(sizeof(Equipement)*reseau->equipement_capacite);
        if (Tab_Temp == NULL) {
            fprintf(stderr, "Erreur malloc pour Equipement.\n");
            return false;
        }
		for(size_t i=0;i<reseau->nb_equipements;i++){
			Tab_Temp[i]=reseau->equipement[i];
		}
		free(reseau->equipement);
		reseau->equipement=Tab_Temp;
	}
    Station st;
	init_station(&st);
	st.mac=*mac;
	st.ip=*ip;
	reseau->equipement[reseau->nb_equipements].valeur.st=st;
	reseau->equipement[reseau->nb_equipements].type=STATION;
    reseau->equipement[reseau->nb_equipements].numero_equipement = reseau->nb_equipements;
	reseau->nb_equipements++;
	return true;
}

bool ajouter_Switch(Reseau_Local *reseau, MACAddress *mac, size_t nb_ports, size_t priorite){
	if(reseau->nb_equipements>=reseau->equipement_capacite){
		reseau->equipement_capacite*=2;
		Equipement* Tab_Temp = malloc(sizeof(Equipement)*reseau->equipement_capacite);
        if (Tab_Temp == NULL) {
            fprintf(stderr, "Erreur malloc pour Equipement.\n");
            return false;
        }
		for(size_t i=0;i<reseau->nb_equipements;i++){
			Tab_Temp[i]=reseau->equipement[i];
		}
		free(reseau->equipement);
		reseau->equipement=Tab_Temp;
	}
    Switch sw;
	init_switch(&sw);
	sw.mac=*mac;
	sw.nb_ports=nb_ports;
	sw.priorite=priorite;
    reseau->equipement[reseau->nb_equipements].valeur.sw = sw;
	reseau->equipement[reseau->nb_equipements].type=SWITCH;
    reseau->equipement[reseau->nb_equipements].numero_equipement = reseau->nb_equipements;
	reseau->nb_equipements++;
	return true;
}

bool ajouter_Liaison(Reseau_Local *reseau, size_t e1, size_t e2, size_t poids){

    //Je pars du principe que les deux équipements existent déjà dans le réseau
    //Vérification que les deux équipements sont différents
    if(e1 == e2){
        fprintf(stderr, "Erreur : les deux équipements sont identiques.\n");
        return false;
    }
    if(e1 >= reseau->nb_equipements || e2 >= reseau->nb_equipements){
        fprintf(stderr, "Erreur numéro d'équipement invalide.\n");
        return false;
    }
    Liaison liaison;
    liaison.e1 = e1;
    liaison.e2 = e2;
    liaison.poids = poids;
    //Vérification que place suffisante dans le tableau de liaisons et si non, taille doublée
    if(reseau->nb_liaisons>=reseau->liaison_capacite){
		reseau->liaison_capacite*=2;
		Liaison* Tab_Temp = malloc(sizeof(Liaison)*reseau->liaison_capacite);
        if (Tab_Temp == NULL) {
            fprintf(stderr, "Erreur malloc pour Liaison.\n");
            return false;
        }
		for(size_t i=0;i<reseau->nb_liaisons;i++){
			Tab_Temp[i]=reseau->liaisons[i];
		}
		free(reseau->liaisons);
		reseau->liaisons=Tab_Temp;
	}
    reseau->liaisons[reseau->nb_liaisons] = liaison;
    reseau->nb_liaisons++;
    return true;
}

//Chargement du réseau à partir du fichier de configuration
int charger_Reseau(Reseau_Local *reseau) {
	//On suppose que  le reseau est déjà initialisé
    size_t nb_equipements = 255;
    size_t nb_liaisons = 255;

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
        if(nligne == 1){
            //Normalement c'est quand même la première ligne à l'indice 1 et pas 0
            sscanf(ligne, "%zu %zu", &nb_equipements, &nb_liaisons);
            if(nb_equipements == 0 || nb_liaisons == 0){
                fprintf(stderr, "Erreur lecture nombre équipements ou  liaisons.\n");
                fclose(fconfig);
                return EXIT_FAILURE;
            }
            printf("Nombre d'équipements : %zu, Nombre de liaisons : %zu\n", nb_equipements, nb_liaisons);
        }
        else if(nligne <= nb_equipements+1){
			//Vérification que place suffisante dans le tableau d'equipements et si non, taille doublée
			if(reseau->nb_equipements>=reseau->equipement_capacite){
				reseau->equipement_capacite*=2;
				Equipement* Tab_Temp = malloc(sizeof(Equipement)*reseau->equipement_capacite);
                if (Tab_Temp == NULL) {
                    fprintf(stderr, "Erreur malloc pour Equipement.\n");
                    exit(EXIT_FAILURE);
                }
				for(size_t i=0;i<reseau->nb_equipements;i++){
					Tab_Temp[i]=reseau->equipement[i];
				}
				free(reseau->equipement);
				reseau->equipement=Tab_Temp;
			}
            if(ligne[0] == '1'){
                //C'est une station
                //Récupération de l'adresse MAC
                MACAddress mac_station;
                init_MacAddress(&mac_station);
                unsigned char mac_octets[6];
                sscanf(ligne + 2, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &mac_octets[0], &mac_octets[1], &mac_octets[2], &mac_octets[3], &mac_octets[4], &mac_octets[5]);
                if(mac_octets[0] == 0 && mac_octets[1] == 0 && mac_octets[2] == 0 && mac_octets[3] == 0 && mac_octets[4] == 0 && mac_octets[5] == 0){
                    fprintf(stderr, "Erreur adresse MAC invalide ou erreur lecture adresse MAC.\n");
                    fclose(fconfig);
                    return EXIT_FAILURE;
                }
                //mac_station->octets=mac_octets;  //Apparement faut memcpy
                memcpy(mac_station.octets, mac_octets, sizeof(mac_octets));
                //Récupération de l'adresse IP
                IPAddrV4 ip_station;
                init_IPAddrV4(&ip_station);
                unsigned char ip_octets[4];
                sscanf(ligne + 20, "%hhu.%hhu.%hhu.%hhu", &ip_octets[0], &ip_octets[1], &ip_octets[2], &ip_octets[3]);
                if(ip_octets[0] == 0 && ip_octets[1] == 0 && ip_octets[2] == 0 && ip_octets[3] == 0){
                    fprintf(stderr, "Erreur adresse IP invalide ou erreur lecture adresse IP.\n");
                    fclose(fconfig);
                    return EXIT_FAILURE;
                }
                //ip_station->octets=ip_octets;    //Pareil memcpy
                memcpy(ip_station.octets, ip_octets, sizeof(ip_octets));
                ajouter_Station(reseau, &mac_station, &ip_station);
            }
            if(ligne[0] == '2'){
                //C'est un switch
                //Récupération de l'adresse MAC
                MACAddress mac_switch;
                init_MacAddress(&mac_switch);
                unsigned char mac_octets[6];
                sscanf(ligne + 2, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &mac_octets[0], &mac_octets[1], &mac_octets[2], &mac_octets[3], &mac_octets[4], &mac_octets[5]);
                if(mac_octets[0] == 0 && mac_octets[1] == 0 && mac_octets[2] == 0 && mac_octets[3] == 0 && mac_octets[4] == 0 && mac_octets[5] == 0){
                    fprintf(stderr, "Erreur adresse MAC invalide ou erreur lecture adresse MAC.\n");
                    fclose(fconfig);
                    return EXIT_FAILURE;
                }
                //mac_switch->octets=*mac_octets;
                memcpy(mac_switch.octets, mac_octets, sizeof(mac_octets));
                //Récupération du nombre de ports
                size_t nb_ports;
                sscanf(ligne + 20, "%zu", &nb_ports);
                if(nb_ports == 0){
                    fprintf(stderr, "Erreur nombre de ports invalide ou erreur lecture nombre de ports.\n");
                    fclose(fconfig);
                    return EXIT_FAILURE;
                }
                //Récupération de la priorité
                size_t priorite;
                sscanf(ligne + 22, "%zu", &priorite);
                if(priorite == 0){
                    fprintf(stderr, "Erreur priorité invalide ou erreur lecture priorité.\n");
                    fclose(fconfig);
                    return EXIT_FAILURE;
                }
                //Initialisation du switch
                ajouter_Switch(reseau, &mac_switch, nb_ports, priorite);
            }
        }
        else if(nligne <= nb_equipements + nb_liaisons+1){
            size_t e1 = UNKNOWN_INDEX;
            size_t e2 = UNKNOWN_INDEX;
            size_t poids = 0;
            //Lecture de la liaison
            printf("%s\n",ligne);
            sscanf(ligne, "%zu;%zu;%zu", &e1, &e2, &poids);
            if(e1 == UNKNOWN_INDEX || e2 == UNKNOWN_INDEX || poids == 0){
                fprintf(stderr, "Erreur lecture liaison ou valeur(s) invalide(s).\n");
                fclose(fconfig);
                return EXIT_FAILURE;
            }
            //Récupération des équipements
            if(!ajouter_Liaison(reseau, e1, e2, poids)){
                fprintf(stderr, "Erreur ajout de la liaison.\n");
                fclose(fconfig);
                return EXIT_FAILURE;
            }
        }
    }
    printf("Lecture du fichier de configuration terminée...\nFermeture du fichier.\n");
    fclose(fconfig);
    //Initialisation du réseau
    return 0;
}
