#ifndef SECTION_H
#define SECTION_H

#include<string>
#include<tuple>
#include "Resource.h"

class Section : public Resource
{
    int _station1;
    int _station2;
	public:
    Section(int id, std::string name, int station1, int station2, int capacity );
    bool get_section_direction(int station1);
    
    friend std::ostream& operator<< (std::ostream& output, Section const& section);
    bool operator==(const std::tuple<int,int> stns);

};

#endif