# SAE Réseaux - Simulation de Réseau

## Le projet

Une application de simulation de réseau local en C avec gestion des protocoles Ethernet et STP.\
Il s'agit d'une application permettant de simuler une architecture réseau, l'envoie et réception de trames Ethernet et l'algorithme de synchronisation Spanning Tree Protocol (STP).

## Techniques utilisées et développement
Programmation en langage C avec structures de données (M23, théorie des graphes).\
Ce projet a été réalisé avec ![Lohan Marchand](https://github.com/Lohan-Marchand) et ![Liam Guilleux--Riff](https://github.com/Saduso)

## Installation
1. Télécharger le repo (qui contient le code source du projet)
2. Compiler le projet avec la commande `make`
3. Lancer l'application avec `./reseau_local [fichier_config]`

## Fonctionnalités

### Étape 1 : Structures de données des équipements

Modélisation des différents équipements d'un réseau local.

**Types de données implémentés :**
- Adresses Ethernet (MAC) : format hexadécimal sur 6 octets
- Adresses IPv4 : notation décimale pointée
- Stations : équipements terminaux du réseau
- Switchs : équipements de commutation avec table d'association

**Caractéristiques d'une station :**
- Adresse MAC (identification Ethernet)
- Adresse IP (identification réseau)

**Caractéristiques d'un switch :**
- Adresse MAC
- Nombre de ports disponibles
- Priorité (utilisée par le protocole STP)
- Table de commutation (association MAC/port)

**Architecture réseau :**
- Représentation sous forme de graphe étiqueté
- Gestion des interconnexions entre équipements
- Pondération des liens selon les débits

### Étape 2 : Fichiers de configuration

Chargement d'architectures réseau depuis des fichiers de configuration au format texte.

**Format du fichier :**
- Première ligne : nombre d'équipements et nombre de liens
- Lignes suivantes : description des équipements
  - Type 1 : station (adresse MAC, adresse IP)
  - Type 2 : switch (adresse MAC, nombre de ports, priorité)
- Dernières lignes : liens d'interconnexion avec pondération

**Pondération des liens :**
- 10 Mb/s : poids = 100
- 100 Mb/s : poids = 19
- 1 Gb/s : poids = 4

### Étape 3 : Commutation de trames Ethernet

Simulation complète de l'échange de trames Ethernet dans le réseau local.

**Structure d'une trame Ethernet :**
- Préambule : 7 octets de synchronisation
- SFD (Start of Frame Delimiter) : 1 octet
- Adresse destination : 6 octets
- Adresse source : 6 octets
- Type de protocole : 2 octets (IPv4, ARP, IPv6, etc.)
- Données (DATA) : 46 à 1500 octets
- FCS (Frame Check Sequence) : 4 octets

**Fonctionnalités d'affichage :**
- Mode utilisateur : présentation claire des informations (source, destination, type)
- Mode hexadécimal : contenu brut des octets de la trame
- Visualisation complète de la structure de la trame

### Étape 4 : Spanning Tree Protocol (STP)

Implémentation du protocole STP pour la configuration automatique du réseau.

**Objectif du protocole :**
- Élimination des boucles dans la topologie réseau
- Configuration automatique des ports des switchs
- Optimisation des chemins de communication

**Processus de synchronisation :**
1. Chargement du réseau depuis un fichier de configuration
2. Initialisation : tables vides, ports en état inconnu
3. Échange de BPDU (Bridge Protocol Data Units) entre switchs
4. Convergence progressive jusqu'à stabilisation

**Configuration finale des ports :**
- **Port racine** : un par switch, orienté vers le switch racine
- **Ports désignés** : ouverts à la transmission
- **Ports bloqués** : fermés pour éviter les cycles

**Algorithme :**
- Élection du switch racine (priorité la plus basse)
- Calcul des chemins optimaux
- Attribution des rôles aux ports
- Blocage sélectif pour couper les cycles

### Architecture de l'application

**Organisation du code :**
- Structures de données modulaires et réutilisables
- Séparation entre modélisation et simulation
- Fonctions de visualisation pour chaque composant
- Makefile pour la compilation

**Bonnes pratiques appliquées :**
- Code clair et commenté
- Nommage explicite des variables
- Organisation modulaire des fichiers sources
- Gestion des erreurs et cas limites

## Remarques techniques

- Le projet s'appuie sur les structures de données (graphes) développées en M23
- Les structures de données sont conçues pour être proches des architectures réelles
