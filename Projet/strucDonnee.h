
#include <cstddef>

typedef size_t* Mac;
typedef size_t* Ip;

typedef struct station
{
	Mac mac;
	Ip ip;
};

typedef struct Switch
{
	Mac mac;
	size_t nbPorts;
	size_t Priorite;
	Ip* commutTab;
};
