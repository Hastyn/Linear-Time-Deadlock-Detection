#include "../include/Train.h"
#include<iostream>
#include <iomanip>

Train::Train(int id, std::string train_id, int priority,int first_event_time,int approx_journey_size)
            :_id(id),_train_id(train_id),_priority(priority),_next_event_time(first_event_time),
            _cur_position(0),_current_resource(NULL),_cur_delay(0),_directon_down(true),_occupied_resources(0)
{
    _journey.reserve(approx_journey_size);
    _journey_steps = 0;
}


bool Train::shrink(){
    _journey.shrink_to_fit();
    return true;
}

int Train::get_id() const{
    return _id;
}

std::string Train::get_name() const{
    return _train_id;
}


int Train::get_next_event_time() const{
    return _next_event_time;
}
int Train::get_priority() const{
    return _priority;
}

bool Train::set_direction(bool is_down){
    _directon_down =is_down;
    return true;
}

bool Train::get_direction() const{
    return _directon_down;
}


bool Train::add_to_journey(int res, int res_type,int min_time, int tt_dep_time, bool direction_down){
    _journey.push_back(Journey(res,res_type,min_time,tt_dep_time,direction_down));
    _journey_steps++;
    return true;
}

bool  Train::update_last_journey_direction(bool direction_down){
    if(_journey.size()==0)
        return -1;
    _journey.back().set_direction(direction_down);
    return true;
}

int Train::get_journey_last_resource()
{
    if(_journey.size()==0)
        return -1;

    return _journey.back().get_resource()._res;
}
JResource Train::get_journey_previous_resource(){
    if(_cur_position == 0)
        return {-1,-1,-1,true};
    return _journey.at(_cur_position-1).get_resource();
}


JResource Train::get_journey_next_resource(){
    if(is_completed())
        return {-1,-1,-1,true};
    return _journey.at(_cur_position+1).get_resource();
}

JResource Train::get_journey_nth_resource(int n) const{
    int pos= _cur_position+n;
    if(pos>=0 && pos<_journey.size())
        return _journey.at(pos).get_resource();
    return {-1,-1,-1,true};
}


Resource const* Train::get_current_resource() const{
    return _current_resource;
}

void Train::set_resources(Resource *cur_resource, Resource *next_resource)
{
    _current_resource = cur_resource;
    _next_resource = next_resource;
    _occupied_resources++;
}

int Train::release_old_resources(){
    int count=_occupied_resources-1;
    _occupied_resources=1;
    return count;
}


bool Train::move(Resource *cur_resource,Resource *next_resource, int track)
{
    _journey.at(_cur_position).set_scheduled_dep_time(_next_event_time);
    _cur_position++;
    Journey& j = _journey.at(_cur_position);
    j.set_track(track);
    int min_time = j.get_min_time();
    if(min_time>0)
    {
        _next_event_time+=min_time;
        int min_dep_time = j.get_tt_dep_constraint();
        if(min_dep_time>_next_event_time)
            _next_event_time = min_dep_time;
    }
    
    _current_resource->initiate_release(_id,_directon_down);
    for(auto _prev_resources:resources_to_release)
    {
        _prev_resources->initiate_release(_id,_directon_down);
    }
    if(is_completed())
        set_resources(NULL,NULL);
    else
    {
        int resource_min_time = next_resource->get_next_event_time();
        if(resource_min_time>_next_event_time)
            _next_event_time = resource_min_time;
        set_resources(cur_resource,next_resource);
    }
        
    set_direction(j.get_direction());
    return true;
}

int Train::halt()
{
    int res_time = _next_resource->get_next_event_time();

    if(res_time<=_next_event_time)
    {
        _next_event_time++;
        _cur_delay++;
    }
    else
    {
        _cur_delay+=res_time-_next_event_time;
        _next_event_time=res_time;
    }

    return _cur_delay;
}

int Train::get_cur_delay() const{ 
    return _cur_delay;
}


void Train::clear_cur_delay()
{
    _cur_delay = 0;
}

bool Train::is_completed(){
    return _cur_position == _journey.size() - 1;
}

int Train::steps_to_destination() const
{
   return _journey_steps-_cur_position;
}

int Train::get_step_delay(int position) const
{
    int pos = _cur_position+position;
    if(pos<0)
        return 0;
    Journey const &journey= _journey.at(pos);
    int schtime  = journey.get_scheduled_dep_time();
    if(schtime==-1)
        schtime = _next_event_time;
    int delay = schtime - journey.get_tt_dep_time();

    return delay;
}

int Train::get_tt_journey_span() const
{
    return _journey.at(_journey_steps-2).get_tt_dep_time() - _journey.front().get_tt_dep_time();
}   


std::tuple<int,int> Train::departure_delay(){
    int steps = _journey.size();
    int delay=0,count =0;
    int st;
    for(int i =1; i<steps-1;i++)
    {
        Journey &journey= _journey.at(i);
        JResource r = journey.get_resource();
        if(r._res_type == 1)
            continue;
        st=journey.get_scheduled_dep_time();
        count++;
        if(st<0){
            delay+=10000; 
        }
        else{
            int d= st - journey.get_tt_dep_time();
            if(d>0){
                delay += d;
            }
        }
    }
    return {count,delay};
}


void Train::perturb(int time){
    for(Journey &journey:_journey)
        journey.perturb(time);
    _next_event_time = _journey[0].get_tt_dep_time();
}

int Train::get_current_pos()
{
    std::cout<<_journey[_cur_position]._res<<" "<<_journey[_cur_position]._res_type<<std::endl;
    return _cur_position;
}

void Train::print_timetable(std::ostream& output){
    int stn_track, sec_track;
    int sch_arr_time, sch_dep_time, tt_dep_time;
    int run_time, halt_time;
    int stn;
    for(Journey const &j:_journey)
    {
        auto r = j.get_resource();
        if(r._res_type==0)
        {
            if(r._res == 0){
                sch_arr_time=j.get_scheduled_dep_time();
            }
            else if(r._res == 1)
            {
                output<<stn<<",";
                output<<stn_track<<",";
                output<<tt_dep_time<<",";
                output<<0<<",";
                output<<halt_time<<",";
                output<<0<<",";
                output<<_train_id<<",";
                output<<_priority<<",";
                output<<sch_arr_time<<",";
                output<<sch_dep_time<<",";
                output<<sch_dep_time<<std::endl;

            }

            stn = r._res;
            stn_track = stn*100 + j.get_track()+1;
            sch_dep_time = j.get_scheduled_dep_time();
            halt_time= j.get_min_time();
            tt_dep_time=j.get_tt_dep_time();
        }
        else
        {
            sec_track= (r._res+1)*100 + j.get_track()+1;
            run_time = j.get_min_time();

            output<<stn<<",";
            output<<stn_track<<",";
            output<<tt_dep_time<<",";
            output<<sec_track<<",";
            output<<halt_time<<",";
            output<<run_time<<",";
            output<<_train_id<<",";
            output<<_priority<<",";
            output<<sch_arr_time<<",";
            output<<sch_dep_time<<",";
            
            sch_arr_time=j.get_scheduled_dep_time();
            output<<sch_arr_time<<std::endl;
        }
    }
}


bool Train::operator==(const std::string &train_id){
    return _train_id.compare(train_id)==0;
}

bool operator<(Train const& train1, Train const& train2){
    if(train1._next_event_time!=train2._next_event_time){
        return train1._next_event_time<train2._next_event_time;
    }

    int t1_free_tracks = train1._current_resource->get_num_free_tracks();
    int t2_free_tracks = train2._current_resource->get_num_free_tracks();
    int t1_state=0,t2_state=0;
    if(t1_free_tracks==0)
        t1_state=2;
    else if(t1_free_tracks==1)
        t1_state=1;
    if(t2_free_tracks==0)
        t2_state=2;
    else if(t2_free_tracks==1)
        t2_state=1;
    if(t1_state!=t2_state){
        return t1_state>t2_state;
    }

    if(train1._priority!=train2._priority)
    {
        return train1._priority<train2._priority;
    }
        
    return train1._id<train2._id;
}

std::ostream& operator<< (std::ostream& output, Train const& train){
    output<<"Id:"<<train._id<<std::endl;
    output<<"TrainId:"<<train._train_id<<std::endl;
    output<<"Priority:"<<train._priority<<std::endl;
    output<<"Direction:"<<train._directon_down<<std::endl;
    output<<"Current Pos:"<<train._cur_position<<std::endl;
    output<<"Journey Size:"<<train._journey.size()<<std::endl;
    output<<std::setw(10)<<"resource"<<std::setw(10)<<"Type"<<std::setw(10)<<"Min Time"<<std::setw(10)<<"Track"<<std::setw(10)<<"Dep Time"<<std::setw(15)<<"Scheduled Time"<<std::endl;
    for(auto const& j : train._journey)
        output<<j<<std::endl;
    output<<"-------"<<std::endl;
}

TrainPriority::TrainPriority(){
    _value=0;
}

TrainPriority::TrainPriority(Train *train):_train(train){
    _value=0;
}

void TrainPriority::set_value(double value)
{
    _value = value;
}

double TrainPriority::get_value()
{
    return _value;
}



bool operator<(TrainPriority const& tp1, TrainPriority const& tp2){
    if(tp1._value<tp2._value)
        return true;
    else if (tp1._value>tp2._value)
        return false;
    else
        return *(tp2._train) < *(tp1._train);
}




void Train::print_timetable_delays(std::ostream& output){
    int stn_track, sec_track;
    int sch_arr_time, sch_dep_time, tt_dep_time;
    int run_time, halt_time;
    int stn;
    output<<std::endl;
    for(Journey const &j:_journey)
    {
        auto r = j.get_resource();
        if(r._res_type==0)
        {
            if(r._res == 0){
                sch_arr_time=j.get_scheduled_dep_time();
            }
            else if(r._res == 1)
            {
                output<<stn<<",";
                output<<_train_id<<",";
                output<<sch_arr_time<<",";
                output<<sch_dep_time<<",";
                output<<tt_dep_time<<",";
                output<<sch_dep_time-tt_dep_time<<std::endl;
                
            }

            stn = r._res;
            stn_track = stn*100 + j.get_track()+1;
            sch_dep_time = j.get_scheduled_dep_time();
            halt_time= j.get_min_time();
            tt_dep_time=j.get_tt_dep_time();
        }
        else
        {
            sec_track= (r._res+1)*100 + j.get_track()+1;
            run_time = j.get_min_time();
                output<<stn<<",";
                output<<_train_id<<",";
                output<<sch_arr_time<<",";
                output<<sch_dep_time<<",";
                output<<tt_dep_time<<",";
                output<<sch_dep_time-tt_dep_time<<std::endl;
                        
            sch_arr_time=j.get_scheduled_dep_time();
        }
    }
}
