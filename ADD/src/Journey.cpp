#include "../include/Journey.h"
#include<iostream>
#include <iomanip>   

Journey::Journey(int res, int res_type, int min_time,int tt_dep_time, bool _direction_down)
                :_res(res), _res_type(res_type), _min_time(min_time),_tt_dep_time(tt_dep_time),_direction_down(_direction_down)
{
    _track=-1;
    _scheduled_dep_time=-1;
}

JResource Journey::get_resource() const{
    return {_res,_res_type,_track,_direction_down};
}

int Journey::get_min_time() const{
    return _min_time;
}

int Journey::get_tt_dep_constraint() const{
    if(_res_type == 0)
        return _tt_dep_time;
    return -1;
}

int Journey::get_tt_dep_time() const{
    return _tt_dep_time;
}

bool Journey::get_direction() const{
    return _direction_down;
}

bool Journey::set_direction(bool direction_down){
    _direction_down = direction_down;
    return true;
}

bool Journey::set_scheduled_dep_time(int time){
    _scheduled_dep_time = time;
    return true;
}

int Journey::get_scheduled_dep_time() const{
    return _scheduled_dep_time;
}

bool Journey::set_track(int track){
    _track = track;
    return true;
}

int Journey::get_track() const{
    return _track;
}

bool Journey::perturb(int time){
    _tt_dep_time+=time;
    return true;
}


std::ostream& operator<< (std::ostream& output, Journey const& journey){
    output<<std::setw(10)<<journey._res<<std::setw(10)<<journey._res_type<<std::setw(10)<<journey._min_time<<std::setw(10)<<journey._track<<std::setw(10)<<journey._tt_dep_time<<std::setw(10)<<journey._scheduled_dep_time<<std::endl;
}