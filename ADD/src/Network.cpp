#include "../include/Network.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <iomanip>

Network::Network(){
}


std::string get_section_name(std::string station1,std::string station2){
    return std::string("Section-")+station1+"-"+station2;
}

int Network::get_station_id(std::string name){
    auto it=std::find(_stations.begin(),_stations.end(),name);
    return it - _stations.begin();
}

int Network::get_section_id( int stn1, int stn2){
    auto it=std::find(_sections.begin(),_sections.end(), std::make_tuple(stn1,stn2));
    return it - _sections.begin();
}

int Network::get_train_id(std::string train_id){
    auto it=std::find(_trains.begin(),_trains.end(),train_id);
    return it - _trains.begin();
}


Resource& Network::get_resource_by_type(int res, int res_type){
    
    if(res_type == 0)
        return _stations.at(res);
    if(res_type == 1)
        return _sections.at(res);
}

Resource const& Network::get_resource_by_type(int res, int res_type) const{
    
    if(res_type == 0)
        return _stations.at(res);
    if(res_type == 1)
        return _sections.at(res);
}


bool Network::reserve(int num_trains, int num_stations, int num_sections)
{
    _trains.reserve(num_trains);
    _stations.reserve(num_stations);
    _sections.reserve(num_sections);
    return true;
}

bool Network::shrink()
{
    _trains.shrink_to_fit();
    _stations.shrink_to_fit();
    _sections.shrink_to_fit();
    for(auto &train:_trains)
        train.shrink();
    return true;
}

Network::Network(const Network &network)
{
    _trains = network._trains;
    _stations = network._stations;
    _sections = network._sections;
    _deadlock_patience = network._deadlock_patience;
    for(Train& train:_trains){
        JResource jres = train.get_journey_next_resource();
        train.set_resources(&_stations.at(0),&get_resource_by_type(jres._res,jres._res_type));
        _stations.at(0).occupy(train.get_id(),true);
    }
}

void Network::set_deadlock_patience(int patience){
    _deadlock_patience = patience;
}

int Network::add_station(std::string name, int capacity, int sub_network)
{
    int id=_stations.size();
    _stations.push_back(Station(id,name,capacity,sub_network));
    
   return id;
}

int Network::add_section(std::string station1,std::string station2, int capacity){

    int stn1=get_station_id(station1);
    int stn2=get_station_id(station2);
    int id=_sections.size();

    _sections.push_back(Section(id,get_section_name(station1,station2),stn1,stn2,capacity));

    return id;
}

int Network::add_train(std::string train_id, int priority,int first_event_time){
    int sz=_trains.size();
    int id = get_train_id(train_id);
    if(id == sz)
        _trains.push_back(Train(id,train_id,priority,first_event_time, _stations.size()+_sections.size() ));
    return id;
}

bool Network::append_train_journey(int train_index, std::string train_id, std::string resource, int run_time, int halt_time, int arr_time, int dep_time, bool add_section_journey){
    
    if(!(_trains.at(train_index) == train_id))
        train_index = get_train_id(train_id);
    
    Train &train = _trains.at(train_index);
    
    int cur_stn = get_station_id( resource);
    bool direction = true;
    if(add_section_journey)
    {
        int last_stn = train.get_journey_last_resource();
        int sec_id = get_section_id(last_stn, cur_stn);
        direction = _sections.at(sec_id).get_section_direction(last_stn);
        train.update_last_journey_direction(direction);
        train.add_to_journey(sec_id,1,run_time,arr_time,direction);
    }
    train.add_to_journey(cur_stn,0,halt_time,dep_time,direction);
    return true;
}

int Network::get_trains_size()
{
    return _trains.size();
}

Train * Network::get_train(int id){
    return &(_trains.at(id));
}

Train const* Network::get_train(int id) const {
    return &(_trains.at(id));
}

bool Network::release_previous_resource(int train_id){

    Train& train = _trains.at(train_id);
    int res_count= train.release_old_resources();
    int i=0;
    while(res_count+i)
    {
        auto jres = train.get_journey_nth_resource(--i);

        if(jres._res != -1)
        {
            Resource &resource = get_resource_by_type(jres._res,jres._res_type);
            resource.release(train_id,jres._track);
        }
    }
    train.clear_cur_delay();

    return true;
}
int Network::check_occup(int train_id) 
{
    Train  &train=_trains.at(train_id);
    auto jres=train.get_journey_nth_resource(1);
    Resource  &resource = get_resource_by_type(jres._res,jres._res_type);
    return resource.check_occupancy(train_id,jres._direction_down);

}

bool Network::move(int train_id){
    Train& train = _trains.at(train_id);
    
    auto jres = train.get_journey_next_resource();
    Resource &resource = get_resource_by_type(jres._res,jres._res_type);
    int track=-1;
    track=resource.check_occupancy(train_id,jres._direction_down);
    if(track!=-1)
    {
    }
    if(track==-1)
    {
        track = resource.occupy(train_id,jres._direction_down);
        train._movement_flag=false;
    }
    if(track==-1)
    {
       return false;
    }
    if(train._resources_acquired)
    {
        for(int i=2;i<=train._resources_acquired && !train._movement_flag;i++)
        {
            auto jres=train.get_journey_nth_resource(i);
            Resource &resource = get_resource_by_type(jres._res,jres._res_type);
            resource.occupy(train_id,jres._direction_down); 
        }
        train._resources_acquired=0;
        train._movement_flag=true;

    }  
    auto jres_next = train.get_journey_nth_resource(2);
    
    Resource *next_resource = NULL;
    if(jres_next._res!=-1)
        next_resource = &get_resource_by_type(jres_next._res,jres_next._res_type);
    train.move(&resource, next_resource, track);
    resource.update_track_event(track,train.get_next_event_time());

    return true;
}

bool Network::halt(int train_id){
    Train& train = _trains.at(train_id);
    int halts = train.halt();
    if(halts>50000)
    {   
        return false;
    }   
    return true;
}

bool Network::custom_halt(int train_id){
    Train& train = _trains.at(train_id);
    int halts = train.halt();
    if(halts>2000)
    {   
        return false;
    }   
    return true;
}

bool Network::optimized_halt(int train_id){
    Train& train = _trains.at(train_id);
    int halts = train.halt();
    if(halts>5000)
    {   
        return false;
    }   
    return true;
}

ROccupancy update_status_by_direction(ROccupancy ocp, bool dir)
{
    if(!dir){
        int t = ocp.up;
        ocp.up=ocp.down;
        ocp.down=t;
    }
    return ocp;
}

ROccupancy Network::get_train_nth_resources_occupancy(int train_id,int n) const{
    const Train &train = _trains.at(train_id);
    auto jres =  train.get_journey_nth_resource(n);
    ROccupancy ocp = {-1,-1,-1,-1};
    if(jres._res!=-1)
    {
        Resource const &resource = get_resource_by_type(jres._res,jres._res_type);
        ocp= update_status_by_direction(resource.get_occupancy(),jres._direction_down);
    }
    return ocp;
}

ROccupancy Network::get_train_current_resources_status(int train_id) const
{
    const Train &train = _trains.at(train_id);
    Resource const *resource = train.get_current_resource();
    return resource->get_occupancy();
}

int Network::get_train_priority(int train_id) const{
    const Train &train = _trains.at(train_id);
    return train.get_priority();
}


void Network::perturb(int max){
    int r,time;
    for(Train& train:_trains){
        r = rand();
        time = r%(2*max);
        train.perturb(time);
    }
}

bool Network::has_free_tracks_in_opp_direction(Train &train, bool direction, int id, int inc)
{
    int tracks=0;
    while(true)
    {
        JResource jres = train.get_journey_nth_resource(id);
        if(jres._res==-1)
            return true;
        Resource const &resource = get_resource_by_type(jres._res,jres._res_type);
        ROccupancy ocp = resource.get_occupancy();

        if(direction)
            tracks = ocp.down;
        else
            tracks = ocp.up;

        if(ocp.capacity==tracks)
            return false;
        
        if(ocp.capacity > ocp.down+ocp.transitional+ocp.up)
                return true;

        id+=inc;
    }
    return true;
}

bool Network::in_deadlock(Train &train)
{
    bool direction = train.get_direction();
    if(has_free_tracks_in_opp_direction(train,direction,0,-1))
        return false;
    if(has_free_tracks_in_opp_direction(train,!direction,1,1))
        return false;
    return true;
}

std::ostream& operator<< (std::ostream& output, Network const& network){
    output<<"Network display"<<std::endl;
    output<<"Trains:"<<std::endl;
    for(auto const& t : network._trains)
        output<<t<<std::endl;
    output<<"============="<<std::endl;

    output<<"Stations:"<<std::endl;
    for(auto const& s : network._stations)
        output<<s<<std::endl;
    output<<"============="<<std::endl;

    output<<"Sections:"<<std::endl;
    for(auto const& s : network._sections)
        output<<s<<std::endl;
    output<<"============="<<std::endl;
}
