#include "stp.h"

void init_BPDU(TrameEthernet *trame){
	for (size_t i=0;i<7;i++){
		trame->preambule[i]=(char)170;
	}
	trame->sfd=(char)171;
	trame->dest =NULL;
	init_MacAddress(&trame->src);
	trame->type[0]=0x00;
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

void creer_BPDU(TrameEthernet *trame,MACAddress *src, MACAddress *dest, char* donnees){
	init_BPDU(trame);
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
	size_t nb_arrivee=transmettre_trame(reseau, &trame, source_index,source_index);
	deinit_trame(&trame);
	if(nb_arrivee<1){
		printf("Trame non délivrée : destinataire introuvable\n");
	}
	else{
		printf("Trame délivrée %zu fois à destination\n",nb_arrivee);
	}
}
