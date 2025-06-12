#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reseau.h"
#include "trame.h"
#include "graphe.h"

#define CONFIG_FILE "config.txt"

int main() {
    Reseau_Local reseau;
    init_reseau_local(&reseau);  //Charger le réseau à partir du fichier de configuration
	charger_Reseau(&reseau);
    printf("Architecture réseau chargée avec succès !\n");

    char donnees[] = "Bonjour, voici une trame Ethernet !";

    TrameEthernet trame;
    init_trame(&trame);

    creer_trame(&trame, &(reseau.equipement[0].valeur.st.mac), &(reseau.equipement[1].valeur.st.mac), donnees);
    afficher_trame_utilisateur(&trame);
    envoyer_trame(&reseau, &(reseau.equipement[0].valeur.st.mac), &(reseau.equipement[1].valeur.st.mac), donnees);
    deinit_trame(&trame);
    printf("Envoi de la trame terminé.\n");

    deinit_reseau_local(&reseau);
    printf("Réseau local déinitialisé.\n");
    printf("Fin du programme.\n");

    return 0;
}
