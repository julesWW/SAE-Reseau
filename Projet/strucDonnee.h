
#include <cstddef>
#include <cstdint>

typedef size_t* Mac;
typedef size_t* Ip;

typedef struct station
{
	Mac mac;
	Ip ip;
} station;

typedef struct Switch
{
	Mac mac;
	size_t nbPorts;
	size_t Priorite;
	Ip* commutTab;
} Switch;

typedef struct Reseau_Local
{
	size_t nbStations;
	station* stations;
	size_t nbSwitchs;
	Switch* switchs;
} Reseau_Local;

typedef struct Trame_Ethernet
{
	char preambule[7];
	char sfd;
	Mac dest;
	Mac src;
	size_t type;
	char* donnees;
	//J'ai mis un bourrage. Nécessaire ?
	uint8_t bourrage[46];
	char fcs[4];
} Trame_Ethernet;