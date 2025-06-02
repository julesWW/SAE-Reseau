

//Adresse MAC
typedef struct {
    unsigned char octets[6];
} MACAddress;

//Adresse IP
typedef struct IPAddrV4{
    char octet1[4];
    char octet2[4];
    char octet3[4];
    char octet4[4];

} IPAddress;

//Station : adresse MAC + adresse IP
typedef struct Station{
    MACAddress mac;
    IPAddress ip;
} Station;

//Entrée dans une table de commutation
typedef struct TableComm{
    MACAddress mac;
    unsigned int port;
} TableComm;

//Switch : adresse MAC + nombre de ports + priorité + table de commutation
typedef struct Switch{
    MACAddress mac;
    int nb_ports;
    int priorite;
    int nb_entrees;
} Switch;

//Réseau local : contient un ensemble de stations et de switches
typedef struct Reseau_Local{
    int nb_stations;
    int nb_switches;
} Reseau_Local;

typedef struct TrameEthernet
{
    
}TrameEthernet;
