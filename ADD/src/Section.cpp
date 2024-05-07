#include "../include/Section.h"
#include <iostream>
Section::Section(int id,std::string name, int station1, int station2, int capacity)
                :Resource(id,name,capacity),_station1(station1), _station2(station2)
{
    
}

bool Section::get_section_direction(int start_station){
    if(_station2 == start_station)
        return false;
    return true;
}


bool Section::operator==(const std::tuple<int,int> stns){
    int stn1=std::get<0>(stns);
    int stn2=std::get<1>(stns);
    if(stn1 == _station1 && stn2 == _station2)
        return true;
    if(stn1 == _station2 && stn2 == _station1)
        return true;
    return false;
}

std::ostream& operator<< (std::ostream& output, Section const& section){
    output<<"Id:"<<section._id<<std::endl;
    output<<"Name:"<<section._name<<std::endl;
    output<<"Capacity:"<<section._capacity<<std::endl;
    output<<"Station1:"<<section._station1<<std::endl;
    output<<"Station2:"<<section._station2<<std::endl;
    output<<"-------"<<std::endl;
}