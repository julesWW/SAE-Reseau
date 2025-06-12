#pragma once

#include "trame.h"

void init_trame(TrameEthernet *trame){
	for (size_t i=0;i<7;i++){
		trame->preambule[i]=170;
	}
    trame->sfd=171;
    init_MACAddress(&trame->dest);
    init_MACAddress(&trame->src);
    trame->type[0]=0x08;
    trame->type[1]=0x00;
    for (int i = 0; i < 1500; i++) {
    	trame->donnees[i] = 0;
	}
	for (int i = 0; i < 46; i++) {
		trame->bourrage[i] = 0;
	}
    for (int i = 0; i < 4; i++) {
		trame->fcs[i] = 0;
	}
}

void deinit_trame(TrameEthernet *trame){
    trame->sfd=0;
    deinit_MACAddress(&trame->dest);
    deinit_MACAddress(&trame->src);
    trame->type[0]=0;
    trame->type[1]=0;
    for (int i = 0; i < 1500; i++) {
    	trame->donnees[i] = 0;
	}
	for (int i = 0; i < 46; i++) {
		trame->bourrage[i] = 0;
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

}
