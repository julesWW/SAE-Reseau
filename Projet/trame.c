#include "trame.h"

void init_trame(TrameEthernet *trame){
	for (size_t i=0;i<7;i++){
		trame->preambule[i]=(char)170;
	}
	trame->sfd=(char)171;
	init_MacAddress(&trame->dest);
	init_MacAddress(&trame->src);
	trame->type[0]=0x08;
	trame->type[1]=0x00;
	for (int i = 0; i < 1500; i++) {
		trame->donnees[i] = (char)0;
	}
	for (int i = 0; i < 46; i++) {
		trame->bourrage[i] = (char)0;
	}
	for (int i = 0; i < 4; i++) {
		trame->fcs[i] = (char)0;
	}
}

void deinit_trame(TrameEthernet *trame){
	trame->sfd=(char)0;
	trame->type[0]=(char)0;
	trame->type[1]=(char)0;
	for (int i = 0; i < 1500; i++) {
		trame->donnees[i] = (char)0;
	}
	for (int i = 0; i < 46; i++) {
		trame->bourrage[i] = (char)0;
	}
}


void creer_trame(TrameEthernet *trame,MACAddress *src, MACAddress *dest, char* donnees){
	init_trame(trame);
	memcpy(&trame->dest, dest, sizeof(MACAddress));
	memcpy(&trame->src, src, sizeof(MACAddress));
	//data
	size_t len = strlen(donnees);
	if (len > 1500){
		len = 1500;
	}
	memcpy(trame->donnees, donnees, len);
	// bourrage
	if (len < 46) {
		for(size_t i=0;i<46-len;i++){
			trame->bourrage[i]=0;
		}
	}
}

void envoyer_trame(Reseau_Local *reseau, MACAddress *src, MACAddress *dest, char* donnees){
	TrameEthernet trame;
	creer_trame(&trame,src,dest,donnees);

	//Trouver l’indice de l’équipement source
	size_t source_index = UNKNOWN_INDEX ;
	source_index = mac_en_indice(reseau, src);

	if (source_index == UNKNOWN_INDEX) {
		printf("Source inconnue dans le réseau\n");
		return;
	}
	transmettre_trame(reseau, &trame, source_index,source_index);
	deinit_trame(&trame);
}


void afficher_trame_utilisateur(TrameEthernet *trame) {
	printf("\n----- TRAME Utilisateur ----\n");

	printf("Adresse source : ");
	for (int i = 0; i < 6; i++) {
		printf("%02X", trame->src.octets[i]);
		if (i < 5) printf(":");
	}
	printf("\n");

	printf("Addresse destination : ");
	for (int i = 0; i < 6; i++) {
		printf("%02X", trame->dest.octets[i]);
		if (i < 5) printf(":");
	}
	printf("\n");

	printf("Type : 0x%02X%02X\n", trame->type[0], trame->type[1]);

	printf("Données (hexa) : ");
	for (int i = 0; i < 1500; i++) {
		if (trame->donnees[i] == 0) break;
		printf("%02X ", trame->donnees[i]);
	}
	printf("\n");

	printf("------------------------\n");
}

//FLOOD
size_t transmettre_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t actuel_index,size_t preced_index) {
	//Parcourir touts les équipement adjacents à la source
	size_t nb_arrivee=0;

	size_t ea[reseau->nb_liaisons];
	size_t nb_ea= equipement_adjacents(reseau,reseau->equipement[actuel_index],ea);

	for (size_t i = 0; i < nb_ea; ++i) {
		if(ea[i]==preced_index){  //si c'est celui d'où on vient
			continue;   //skip
		}
		if(reseau->equipement[actuel_index].type==SWITCH && reseau->equipement[actuel_index].valeur.sw.table_commutation[i].etat==FERME){ //si on est dans un switch et que ce port est fermé
			continue;   //skip
		}
		Equipement *voisin = &reseau->equipement[ea[i]];

		//Vérifier si c'est une station
		if (voisin->type == STATION) {
			nb_arrivee+=station_receptionne_trame(reseau, trame,ea[i]);
			}
		//Si c'est un switch
		if (voisin->type == SWITCH) {
			Switch *sw = &voisin->valeur.sw;
			nb_arrivee+=switch_receptionne_trame(reseau, trame, actuel_index, sw, ea[i]);
			}
	}
	if(nb_arrivee<1){
		printf("Trame non délivrée : destinataire introuvable\n");
	}
	else{
		printf("Trame délivrée %zu fois à destination\n",nb_arrivee);
	}
	return nb_arrivee;
}

size_t station_receptionne_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t reception_index){
	if (memcmp(reseau->equipement[reception_index].valeur.st.mac.octets, trame->dest.octets, 6) == 0) {
		printf("Trame livrée à la station [%zu]\n", reception_index);
		return 1;
	}
	printf("Trame arrivée à la station [%zu] (mauvaise station)\n", reception_index);
	return 0;
}

size_t mac_en_indice(Reseau_Local *reseau, MACAddress *mac){
	for(size_t i=0;i<ordre(reseau);i++){
		if( reseau->equipement[i].type==SWITCH && memcmp(&reseau->equipement[i].valeur.sw.mac,mac,sizeof(MACAddress))==0){
			return i;
		}
		else if(reseau->equipement[i].type==STATION && memcmp(&reseau->equipement[i].valeur.st.mac,mac,sizeof(MACAddress))==0){
			return i;
		}
	}
    return UNKNOWN_INDEX;
}

size_t switch_receptionne_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t preced_index,Switch *sw, size_t actuel_index){
	//enregistrer mac de equipement précédant
	size_t ea[reseau->nb_liaisons]; //alloue assez de place dans ea pour les equipement adjacents
	size_t nb_ea=equipement_adjacents(reseau,reseau->equipement[actuel_index],ea);//enregistre les equipement adjacents dans ea (et leurs nombre dans nb_ea)
	for (size_t i = 0; i < nb_ea; ++i) {//pour chaque equipement adjacents
		if(ea[i]==preced_index){//si l'index de equipement adjacents num i est le même que l'index de l'équipement précédant (alors i = indice du port d'entrée)
			if(reseau->equipement[preced_index].type==STATION){   //si c'est une station
				memcpy(&sw->table_commutation[i].mac,&reseau->equipement[preced_index].valeur.st.mac,sizeof(MACAddress));   //copie de l'adresse mac dans la table de commutation
			}
			else if(reseau->equipement[preced_index].type==SWITCH){   //si c'est un switch
				memcpy(&sw->table_commutation[i].mac,&reseau->equipement[preced_index].valeur.sw.mac,sizeof(MACAddress));   //copie de l'adresse mac dans la table de commutation
			}
		}
	}

	//Transmission
	printf("Trame diffusée par switch [%zu]\n", actuel_index); //affiche info chemin
	for (size_t i = 0; i < sw->nb_ports; ++i) {  //pour chaque ports
		if (memcmp(&sw->table_commutation[i].mac.octets, trame->dest.octets, 6) == 0) {   //si le port contient l'adresse mac recherché
			size_t port_dest = sw->table_commutation[i].port; //enregistre num du port dans port_dest
			printf("addresse mac de la destination au port [%zu]\n", port_dest);  //affiche info chemin
			size_t dest_index=mac_en_indice(reseau,&sw->table_commutation[i].mac);  //enregistre indice de l'equipement du port dans dest_index
			if(reseau->equipement[dest_index].type==STATION){   //si station
				return station_receptionne_trame(reseau, trame, dest_index);   //station reçois trame
			}
			else if(reseau->equipement[dest_index].type==SWITCH){   //si switch
				return switch_receptionne_trame(reseau, trame, actuel_index, &reseau->equipement[dest_index].valeur.sw, dest_index);   //switch reçois trame
			}
		}
	}
	//si mac pas reconnu
	printf("MAC inconnue -> flood\n");//affiche info chemin
	return transmettre_trame(reseau, trame, actuel_index, preced_index);//Flood
}
