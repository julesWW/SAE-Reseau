#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef size_t* Mac;
typedef size_t* Ip;

typedef struct station
{
	Mac mac;
	Ip ip;
}
