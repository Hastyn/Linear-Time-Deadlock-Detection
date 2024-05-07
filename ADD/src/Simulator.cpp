#include "../include/Simulator.h"
#include <fstream>
#include <iostream>
#include <string>
#include <bits/stdc++.h> 
#include <ctime>
#include <cstdlib>

#include "../include/Scheduler_Custom.h"
#include "../include/Scheduler_PolicyImprovement.h"
#include "../include/Scheduler_OptimizedPolicyImprovement.h"
#include "../include/Scheduler.h"
#include "../include/Network.h"

#define STATIONS_COLUMNS 3
#define SECTIONS_COLUMNS 3
#define TIMETABLE_COLUMNS 15
#define TIMETABLE_COLUMNS_REQD 8

// #define FILE_DIR std::string("data")
#define START_STATION std::string("START")
#define END_STATION std::string("END")


time_t get_time_from_string(std::string time)
{
    std::stringstream ss(time);
    struct std::tm t={};
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    return std::mktime(&t);
}


bool init_stations_from_file(std::string file_path, int num_stations, Network &ntw)
{
    std::ifstream fin;
    fin.open(file_path, std::ios::in);

    std::string line, words[STATIONS_COLUMNS];
    
    int station_index, capc_index, subnetwork_index;
    int i=0,rows=0;
    while(getline(fin,line)){
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if ( line.empty()) continue;
        std::stringstream str(line);
        while(std::getline(str, words[0],',')){
            if(words[0].compare("Station")==0)
                station_index=i;
            else if(words[0].compare("Capc")==0)
                capc_index=i;
            else if(words[0].compare("SubNetwork")==0)
                subnetwork_index=i;
            else
                throw std::runtime_error(std::string( "Invalid File header found- ")+file_path+":"+words[0]);
            i++;
        }
        if(i!=STATIONS_COLUMNS)
            throw std::runtime_error(std::string("Invalid File header found- ")+file_path+":"+line);
        break;
    }
    while(getline(fin,line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if (line.empty()) continue;
        
        std::stringstream str(line);
        i=0;

        while(std::getline(str, words[i++],','));
        ntw.add_station(words[station_index],stoi(words[capc_index]),stoi(words[subnetwork_index]));
        rows++;
    }
    if(rows!=num_stations)
        throw std::runtime_error(std::string("Invalid Data: stations count in configuration and file do not match")+ std::to_string(num_stations) + " " + std::to_string(rows));
    
    return true;
}

bool init_sections_from_file(std::string file_path, int num_sections, Network &ntw)
{
    std::ifstream fin;
    fin.open(file_path, std::ios::in);

    std::string line, words[SECTIONS_COLUMNS];
    
    int station1_index,station2_index, capc_index;
    int i=0,rows=0;
    while(getline(fin,line)){
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if ( line.empty()) continue;
        std::stringstream str(line);
        while(std::getline(str, words[0],',')){
            if(words[0].compare("Station1")==0)
                station1_index=i;
            else if(words[0].compare("Station2")==0)
                station2_index=i;
            else if(words[0].compare("Capc")==0)
                capc_index=i;
            else
                throw std::runtime_error(std::string( "Invalid File header found- ")+file_path+":"+words[0]);
            i++;
        }
        if(i!=SECTIONS_COLUMNS)
            throw std::runtime_error(std::string("Invalid File header found- ")+file_path+":"+line);
        break;
    }
    
    while(getline(fin,line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if (line.empty()) continue;
        std::stringstream str(line);
        i=0;

        while(std::getline(str, words[i++],','));
        ntw.add_section(words[station1_index], words[station2_index], stoi(words[capc_index]));
        rows++;
    }
    if(rows!=num_sections)
        throw std::runtime_error(std::string("Invalid Data: sections count in configuration and file do not match")+ std::to_string(num_sections) + " " + std::to_string(rows));
    
    return true;
}

bool init_timetable_from_file(std::string file_path, int num_trains, Network &ntw, time_t start_time)
{
    std::ifstream fin;
    fin.open(file_path, std::ios::in);

    std::string line, words[TIMETABLE_COLUMNS];
   
    int station_index, arr_time_index,dept_time_index, halt_time_index, run_time_index, train_index, priority_index, subnetwork_index;
    int i=0,j=0;
    while(getline(fin,line)){
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if ( line.empty()) continue;
        std::stringstream str(line);
        while(std::getline(str, words[0],',')){
            if(words[0].compare("Station")==0)
                station_index=i;
            else if(words[0].compare("TTArrTime")==0)
                arr_time_index=i;
            else if(words[0].compare("TTDepTime")==0)
                dept_time_index=i;
            else if(words[0].compare("MinHaltTime")==0)
                halt_time_index=i;
            else if(words[0].compare("MinRunTime")==0)
                run_time_index=i;
            else if(words[0].compare("TrainID")==0)
                train_index=i;
            else if(words[0].compare("Priority")==0)
                priority_index=i;
            else if(words[0].compare("SubNetwork")==0)
                subnetwork_index=i;
            else
                j--;
            j++;
            i++;
        }
        if( j!= TIMETABLE_COLUMNS_REQD)
            throw std::runtime_error(std::string("Invalid File header found- ")+file_path+":"+line);
        break;
    }
    std::string train_ids[num_trains];
    int run_time[num_trains]={0};
    int train_list_index=-1,train_count=0;
    bool add_section_in_journey=false;
    
    while(getline(fin,line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if (line.empty()) continue;

        std::stringstream str(line);
        i=0;

        while(std::getline(str, words[i++],','));

        int arr_time = std::difftime(get_time_from_string(words[arr_time_index]),start_time)/60;
        int dept_time = std::difftime(get_time_from_string(words[dept_time_index]),start_time)/60;

        if(train_count == 0 || train_ids[train_count-1].compare(words[train_index])!=0)
        {
            int new_index = ntw.add_train(words[train_index],stoi(words[priority_index]),arr_time);
            if(new_index>train_list_index){
                train_ids[train_count]=words[train_index];
                ntw.append_train_journey(new_index,words[train_index],START_STATION,0,0,0,arr_time,false);
                train_count++;
                add_section_in_journey=false;
            }
            else
                add_section_in_journey=true;
            train_list_index=new_index;
        }
        else 
            add_section_in_journey=true;
        ntw.append_train_journey(train_list_index,train_ids[train_count-1],words[station_index],run_time[train_list_index],stoi(words[halt_time_index]),arr_time,dept_time,add_section_in_journey);
        run_time[train_list_index] = stoi(words[run_time_index]);
    }

    if(train_count!=num_trains)
        throw std::runtime_error(std::string("Invalid Data: Train count in configuration and file do not match")+ std::to_string(num_trains) + " " + std::to_string(train_count));
    
    for(i=0;i<num_trains;i++)
        ntw.append_train_journey(i,train_ids[i],END_STATION,0,0,0,0,false);

    return true;
}


void  init_default_stations(Network &ntw, int ntrains){
    ntw.add_station(START_STATION,2*ntrains+10,-1);
    ntw.add_station(END_STATION,2*ntrains+10,-1);
}

void initNetwork(SimulatorConfig &config,std::string data_path, Network &ntw)
{
    ntw.set_deadlock_patience(config.deadlock_patience);
    ntw.reserve(config.num_trains,config.num_stations,config.num_sections);
    init_default_stations(ntw, config.num_trains);
    if(!init_stations_from_file(data_path+config.stations_file,config.num_stations, ntw))
        throw std::string("Error in initializing stations from file");

    if(!init_sections_from_file(data_path+config.sections_file,config.num_sections, ntw))
        throw std::string("Error in initializing sections from file");
    
    if(!init_timetable_from_file(data_path+config.timetable_file,config.num_trains, ntw, config.start_time))
        throw std::string("Error in initializing sections from file");

    ntw.shrink();    
}
void update_simulator_config(std::string config_file, SimulatorConfig &config)
{
    std::ifstream fin;
    fin.open(config_file, std::ios::in);
    std::string line, word;

    while (getline(fin, line)) 
    {
        line.erase(std::remove_if(line.begin(), line.end(), isspace),line.end());
        if (line[0] == '#' || line.empty()) continue;

        auto delimiter_pos = line.find("=");
        auto key = line.substr(0, delimiter_pos);
        auto value = line.substr(delimiter_pos + 1);

        if (key == "stations") 
            config.num_stations = stoi(value);
        else if (key == "sections") 
            config.num_sections = stoi(value);
        else if (key == "trains") 
            config.num_trains = stoi(value);
        else if (key == "stations_file") 
            config.stations_file = value;
        else if (key == "sections_file") 
            config.sections_file = value;
        else if (key == "timetable_file") 
            config.timetable_file = value;
        else if (key == "deadlock_patience") 
            config.deadlock_patience = stoi(value);
        else if (key == "start_time") 
            config.start_time = get_time_from_string(value);
    }
}

Simulator::Simulator(std::string data_path, std::string config_file, std::string timetable_file)
{
    SimulatorConfig config;
    update_simulator_config(config_file,config);
    
    if(!timetable_file.empty())
        config.timetable_file = timetable_file;
    num_sections=config.num_sections;
    num_stations=config.num_stations;
    num_trains=config.num_trains;
    start_time=config.start_time;

    initNetwork(config, data_path, _base_network);   
}

Simulator::Simulator(std::string data_path, std::string config_file, std::string timetable_file, bool custom)
{
    SimulatorConfig config;
    update_simulator_config(config_file,config);
    
    if(!timetable_file.empty())
        config.timetable_file = timetable_file;
    num_sections=config.num_sections;
    num_stations=config.num_stations;
    num_trains=config.num_trains;
    start_time=config.start_time;

    initNetwork(config, data_path, _base_network);
    
}

void Simulator::printStationNames(Network &network)
{
    for(auto station:network._stations)
    {
        std::cout<<station<<std::endl;
    }
}



Scheduler * get_scheduler(int type, Network &network, float threshold)
{
    Scheduler *scheduler;
    switch (type)
    {
    case 0:
        scheduler = new Scheduler(network);
        break;
    case 1:
        scheduler = new Scheduler_Custom(network);
        break;
    case 2: 
        scheduler = new Scheduler_PolicyImprovement(network);
        break;
    case 3: 
        scheduler = new Scheduler_OptimizedPolicyImprovement(network);
        break;
    default:
        scheduler = new Scheduler(network);
        break;
    }
    return scheduler;
        
}

std::tuple<bool,float> Simulator::run(Solution &solution, int scheduler_type, int seed, int max_perturb, float threshold)
{
    Network network= _base_network;
    if(seed >=0)
        std::srand(seed);
    if(max_perturb >0)
        network.perturb(max_perturb);
    
    Scheduler *scheduler = get_scheduler(scheduler_type,network,threshold);
    bool success = scheduler->run(solution);
    scheduler->save_timetable("runs/_timetable.csv");
    scheduler->compare_delays();
    
    float pwdd = scheduler->get_pwdd();
    free(scheduler);
    return {success,pwdd};
}
