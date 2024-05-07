#include "../include/Resource.h"

#include<iostream>
#include <algorithm>

Resource::Resource(int id,std::string name, int capacity)
            :_id(id), _name(name), _capacity(capacity),_occupied_up(0),_occupied_down(0),_occupied_transitional(0)
{
    _tracks.resize(capacity);
    std::fill(_tracks.begin(),_tracks.end(),TrainEvent());
}
int Resource::get_id() const{
    return _id;
}
std::string Resource::get_name() const{
    return _name;
}

void Resource::update_next_event(){
    
    TrainEvent event = *std::max_element(_tracks.begin(),_tracks.end());
   _next_event.set(event._train_id,event._time); 
    
}


bool Resource::initiate_release(int train_id, bool direction_down){
    if(direction_down){
        _occupied_down--;
    }
    else
    {
        _occupied_up--;
    }
    _occupied_transitional++;
    return true;
}

void Resource::release(int train_id, int track){
    _occupied_transitional--;
    if(track>=0)
        _tracks.at(track).reset();
    update_next_event();
}

int Resource::occupy(int train_id, bool direction_down){
    if(get_num_free_tracks() == 0)
        return -1;

    int track=-1;
    if(direction_down)
    {
        _occupied_down++;
        auto it = std::find(_tracks.begin(),_tracks.end(),-1);
        if(it != _tracks.end())
            track = it-_tracks.begin();
    }
    else
    {
        _occupied_up++;
        auto it = std::find(_tracks.rbegin(),_tracks.rend(),-1);
        if(it != _tracks.rend())
            track =_tracks.rend() - it -1;
    }
    if(track>=0)
    {
        _tracks.at(track).set(train_id);
    }
    return track;
}
int Resource::check_occupancy(int train_id,bool direction_down) 
{
    if(direction_down)
    {
        auto it = std::find(_tracks.begin(),_tracks.end(),train_id);
        if(it != _tracks.end())
            return it-_tracks.begin();
        return -1;
   }
    else
    {
        auto it = std::find(_tracks.rbegin(),_tracks.rend(),train_id);
        if(it != _tracks.rend())
            return _tracks.rend() - it -1;
        return -1;
    }
}


void Resource::update_track_event(int track, int time){
   // std::cout<<_tracks.size();
    _tracks.at(track).set_time(time);
    update_next_event();
}

int Resource::get_next_event_time() const
{
    return _next_event._time;
}

int Resource::get_num_free_tracks() const
{
    return _capacity - _occupied_down - _occupied_up -_occupied_transitional;
}

ROccupancy Resource::get_occupancy() const
{
    return {_capacity,_occupied_up,_occupied_down,_occupied_transitional};
}


bool Resource::operator==(const std::string &name){
    return _name.compare(name)==0;
}