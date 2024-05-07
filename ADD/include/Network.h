#ifndef NETWORK_H
#define NETWORK_H

#include<vector>
#include<string>
#include "Train.h"
#include "Station.h"
#include "Section.h"

class Network
{
    //Add new properties in copy constructor
    std::vector<Section> _sections;
    int _deadlock_patience;
    bool has_free_tracks_in_opp_direction(Train &train, bool direction, int id, int inc);
	public:
    std::vector<Station> _stations;
    
    std::vector<Train> _trains;
    
    Network();
    Network(const Network &network);
    bool reserve(int num_trains, int num_stations, int num_sections);
    bool shrink();
    void set_deadlock_patience(int patience);
    int get_station_id(std::string name);
    int get_section_id( int stn1, int stn2);
    int get_train_id(std::string train_id);

    int add_station(std::string name, int capacity, int sub_network);
    int add_section(std::string station1,std::string station12, int capacity);
    int add_train(std::string train_id, int priority,int first_event_time);
    bool append_train_journey(int train_index, std::string train_id, std::string resource, int run_time, int halt_time, int arr_time, int dep_time, bool add_section_journey);
    int get_trains_size();
    Train * get_train(int id);
    Train const* get_train(int id) const;
    bool release_previous_resource(int train_id);

    bool move(int train_id);
    bool halt(int train_id);
    bool custom_halt(int train_id);
    bool optimized_halt(int train_id);

    bool in_deadlock(Train &train);
    void print_res_status();
    Resource& get_resource_by_type(int res, int res_type);
    Resource const& get_resource_by_type(int res, int res_type) const;

    ROccupancy get_train_nth_resources_occupancy(int train_id,int n) const;
    ROccupancy get_train_current_resources_status(int train_id) const;
    int get_train_priority(int train_id) const;

	void perturb(int max);

    friend std::ostream& operator<< (std::ostream& output, Network const& network);
    int check_occup(int train_id) ;
};

#endif