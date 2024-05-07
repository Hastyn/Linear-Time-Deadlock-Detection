#include "../include/Station.h"
#include<iostream>

Station::Station(int id,std::string name, int capacity, int sub_network)
            :Resource(id,name,capacity), _sub_network(sub_network)
{
}

std::ostream& operator<< (std::ostream& output,  Station const& station){
    output<<"Id:"<<station._id<<std::endl;
    output<<"Name:"<<station._name<<std::endl;
    output<<"Capacity:"<<station._capacity<<std::endl;
    output<<"Up:"<<station._occupied_up<<std::endl;
    output<<"Down:"<<station._occupied_down<<std::endl;
    output<<"-------"<<std::endl;
}