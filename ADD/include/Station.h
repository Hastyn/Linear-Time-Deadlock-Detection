#ifndef STATION_H
#define STATION_H

#include<string>
#include "Resource.h"

class Station : public Resource
{
    int _sub_network;

	public:
    Station(int id,std::string name, int capacity, int sub_network);

    friend std::ostream& operator<< (std::ostream& output, Station const& station);
};

#endif