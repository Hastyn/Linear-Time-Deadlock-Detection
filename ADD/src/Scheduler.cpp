#include "../include/Scheduler.h"
#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 
Scheduler::Scheduler(Network &network):_network(network),_cur_timestep(0)
{
    initialize_pq();
}

void Scheduler::initialize_pq()
{
    int num_trains = _network.get_trains_size();
    for(int i=0;i<num_trains;i++){
        Train *train = _network.get_train(i);
        _pq.emplace(train->get_id(),train->get_next_event_time());
    }
}


void Scheduler::release_moved_trains(){

    for(auto id:_moved_trains){
        _network.release_previous_resource(id);
    }
    _moved_trains.clear();
}

int Scheduler::get_current_trains(){
    if(_pq.empty())
        return -1;
    TrainEvent event = _pq.top();
    int event_time = event._time;
    int next_event_time=0;
    do{
        _pq.pop();
        _current_trains.emplace_back(_network.get_train(event._train_id));
        if(_pq.empty())
            break;
        event =  _pq.top();
        next_event_time = event._time;
    }while(event_time==next_event_time);
    return event_time;
}

bool Scheduler::on_move(Train *train)
{
    int train_id = train->get_id();
    if(_network.move(train_id))
    {
        _moved_trains.push_back(train_id);
        if(!train->is_completed())
            _pq.emplace(train_id,train->get_next_event_time());
        return true;
    }
    return false;
    
}

bool Scheduler::on_halt(Train *train)
{
    bool success = _network.halt(train->get_id());
    if(success)
        _pq.emplace(train->get_id(),train->get_next_event_time());
    return success;
}

TrainPriority Scheduler::get_next_current_train()
{
    auto it = std::max_element(_current_trains.begin(),_current_trains.end());
    auto tp = *it;
    _current_trains.erase(it);
    return tp;
}

bool Scheduler::getPolicyAction(Solution &solution, TrainPriority &tp)
{
    int t=solution.policy(tp._train->get_id());
    tp._train->_resources_acquired=t;
    if(_network.check_occup(tp._train->get_id())!=-1)
        return true;
    if(t==0)
    {
        return false;
    }
    else{
        return true;
    }
}

void Scheduler::sendPolicyAck(Solution &solution, int train_id, bool policy, bool moved )
{
    solution.policy_ack(train_id, _cur_timestep, policy, moved);
}

bool Scheduler::run(Solution &solution)
{
    solution.set_network(&(_network));
    _solution = &solution;
    int halt_times = 0,last_timestep=0;
    _completed=false;
    _deadlock =false;
    while(!_completed && !_deadlock){
     
        last_timestep = _cur_timestep;
        _cur_timestep = get_current_trains();
        if(last_timestep != _cur_timestep && !_moved_trains.empty())
        {
            release_moved_trains();
        }
        
        if(_cur_timestep == -1){
            _cur_timestep=last_timestep;
            if(_pq.size()!=0)
                std::cout<<"Error: PQ is not empty "<<_cur_timestep<<std::endl;
            _completed = true;
            break;
        }
        while(!_current_trains.empty() && !_deadlock)
        {
            auto tp = get_next_current_train();
            if(tp._train == NULL)
                continue;

            int train_id = tp._train->get_id();
            bool moved = false;
            
            int policy = getPolicyAction(solution,tp);

            if(policy)
            {
                moved = on_move(tp._train);
            }
            if(!moved)
            {
                _deadlock = !on_halt(tp._train);
            }
            sendPolicyAck(solution, train_id, policy, moved);
          
        }
    }

    solution.complete(_completed,_deadlock);

    return _completed && !_deadlock;
}

void Scheduler::save_timetable(std::string filename)
{
    std::ofstream wf(filename);
    wf<<"Station,Loop,TTDepTime,Secn,MinHaltTime,MinRunTime,TrainID,Priority,SCArrTime,SCDepTime,SecnExitTime"<<std::endl;
    for(Train &train:_network._trains)
    {
        train.print_timetable(wf);
    }
    wf.close();
}

void Scheduler::compare_delays()
{
    std::ofstream wf("runs/delays_run.csv");
    wf<<"TrainID,SCArrTime,SCDepTime,TTDepTime,delay";
    for(Train &train:_network._trains)
    {
        train.print_timetable_delays(wf);
    }
    wf.close();
}


float Scheduler::get_pwdd(){
    float total=0;
    float delay;
    int priority;
    int c=0,count=0;
    for(Train &train:_network._trains)
    {
        std::tie(c,delay) = train.departure_delay();
        priority = train.get_priority();
        total += (delay/priority);
        count+=c;
    }
    return total/count;
}

