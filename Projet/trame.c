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

	//Trouver l’indice de l’équipement source
    size_t source_index = -1;
    for (size_t i = 0; i < reseau->nb_equipements; ++i) {
        Equipement *eq = &reseau->equipement[i];
        if ((eq->type == STATION && memcmp(eq->valeur.st.mac.octets, src->octets, 6) == 0) ||
            (eq->type == SWITCH && memcmp(eq->valeur.sw.mac.octets, src->octets, 6) == 0)) {
            source_index = i;
            break;
        }
    }

    if (source_index == (size_t)-1) {
        printf("Source inconnue dans le réseau\n");
        return;
    }

    transmettre_trame(reseau, &trame, source_index);

}


void afficher_trame_utilisateur(TrameEthernet *trame) {
    printf("\n----- TRAME User ----\n");

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

void transmettre_trame(Reseau_Local *reseau, TrameEthernet *trame, size_t source_index) {
    //Parcourir toutes les liaisons à partir de la source
    for (size_t i = 0; i < reseau->nb_liaisons; ++i) {
        Liaison *liaison = &reseau->liaisons[i];
        
        size_t voisin_index = -1;

        if (liaison->e1 == source_index) {
            voisin_index = liaison->e2;
        } else if (liaison->e2 == source_index) {
            voisin_index = liaison->e1;
        } else {
            continue;
        }

        Equipement *voisin = &reseau->equipement[voisin_index];

        //Vérifier si c'est une station
        if (voisin->type == STATION) {
            if (memcmp(voisin->valeur.st.mac.octets, trame->dest.octets, 6) == 0) {
                printf("Trame livrée à la station [%zu]\n", voisin_index);
                return;
            }
        }

        //Si c'est un switch, on flood les trames sur tous les ports
        if (voisin->type == SWITCH) {
            Switch *sw = &voisin->valeur.sw;

            bool mac_connue = false;
            for (size_t j = 0; j < sw->nb_ports; ++j) {
                if (memcmp(sw->table_commutation[j].mac.octets, trame->dest.octets, 6) == 0) {
                    size_t port_dest = sw->table_commutation[j].port;
                    printf("Trame transmise par switch [%zu] via port [%u]\n", voisin_index, port_dest);
                    //Appel récursif possible ici vers l’équipement lié à ce port (à implémenter proprement plus tard)
                    mac_connue = true;
                    break;
                }
            }

            if (!mac_connue) {
                printf("Trame diffusée par switch [%zu] (MAC inconnue)\n", voisin_index);
                //On pourrait envoyer vers tous les ports sauf le port d’entrée
            }
        }
    }

    printf("Trame non délivrée : destinataire introuvable\n");
}
