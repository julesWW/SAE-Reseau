#include "reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

//Fonctions d'initialisation et de deinitialisation
void init_MacAddress(MACAddress *mac) {
    mac->octets=malloc(sizeof(unsigned char)*6);
}

void deinit_MacAddress(MACAddress *mac){
	free(mac->octets);
	mac->octets=NULL;
}

void init_IPAddrV4(IPAddrV4 *ip) {
    ip->octets=malloc(sizeof(unsigned char)*4);
}

void deinit_IPAddrV4(IPAddrV4 *ip){
	free(ip->octets);
	ip->octets=NULL;
}

void init_station(Station *station) {
    init_MACAddress(station->mac) ;
    init_IPAddrV4(station->ip) ;
}

void deinit_station(Station *station) {
	deinit_MacAddress(station->mac);
	deinit_IPAddrV4(station->ip);
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
    sw->nb_entrees = 0;
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
    sw->nb_entrees = 0;
    free(sw->table_commutation);
    sw->table_commutation=NULL;
}

void init_reseau_local(Reseau_Local *reseau) {
    reseau->nb_stations = 0;
    reseau->station = malloc(INITIAL_CAPACITY * sizeof(Station));// allocation d'un tableau de Station de capacité initiale de 8
    reseau->nb_switchs = 0;
    reseau->switchs = malloc(INITIAL_CAPACITY * sizeof(Switch));// allocation d'un tableau de Switch de capacité initiale de 8
}

void deinit_reseau_local(Reseau_Local *reseau) {
    reseau->nb_stations = 0;
    // Il faut probablement ajouter une boucle qui deinit chaque stations ,pareil pour les switchs ci-dessous et les TableComm dans les switchs
    free(reseau->station);
    reseau->station=NULL;
    reseau->nb_switchs = 0;
    free(reseau->switchs);
    reseau->switchs = NULL;
}

void init_TrameEthernet(TrameEthernet *trame) {
	trame->preambule=malloc(7*sizeof(char));
    trame->sfd = 0; //pas sur du 0
    trame->dest = malloc( 6 * sizeof(MACAddress));
    trame->src = malloc( 6 * sizeof(MACAddress));
	trame->type=malloc(7*sizeof(char));
	trame->donnees=malloc(1500*sizeof(char));
	trame->bourrage=malloc(46*sizeof(char));
	trame->fcs=malloc(4*sizeof(char));
	//pas sur des valeurs
}

void deinit_TrameEthernet(TrameEthernet *trame) {
	free(trame->preambule);
	trame->preambule=NULL;
    trame->sfd = 0; //pas sur du 0
	free(trame->dest);
	trame->dest=NULL;
	free(trame->src);
	trame->src=NULL;
	free(trame->type);
	trame->type=NULL;
	free(trame->donnees);
	trame->donnees=NULL;
	free(trame->bourrage);
	trame->bourrage=NULL;
	free(trame->fcs);
	trame->fcs=NULL;
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

//Chargement du réseau à partir du fichier de configuration
int charger_Reseau(Reseau_Local *reseau) {
	//on suppose que  le reseau est déjà initialisé
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
