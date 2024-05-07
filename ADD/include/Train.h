#ifndef TRAIN_H
#define TRAIN_H
#include <vector> 
#include <string>
#include "Journey.h"
#include "Resource.h"

class Train
{
	private:
		int _id;
		std::string _train_id;
		int _priority;
        std::vector<Journey> _journey;
		int _cur_position;
		int _next_event_time;
		int _occupied_resources;
		int _cur_delay;
		Resource *_current_resource;
		Resource *_next_resource;
		bool _directon_down;
		int _journey_steps;
		std::vector<Resource*>resources_to_release;
    public:
		int _resources_acquired=0;
		int _movement_flag=false;
		Train(int id, std::string train_id, int priority,int first_event_time,int approx_journey_size);
		bool shrink();
        std::string get_name() const;
		int get_id() const;
        int get_next_event_time() const;
		int get_priority() const;
		//int get_curent_position() const;
        bool add_to_journey(int res, int res_type,int min_time, int tt_dep_time, bool direction_down);
		bool update_last_journey_direction(bool direction_down);
		int get_journey_last_resource();
		JResource get_journey_previous_resource();
		JResource get_journey_next_resource();
		JResource get_journey_nth_resource(int n) const;
    
		int release_old_resources();

		Resource const* get_current_resource() const;
		
		void set_resources(Resource *cur_resource, Resource *next_resource);

		bool set_direction(bool is_down);
		bool get_direction() const;
		int get_current_pos();
		bool move(Resource *cur_resource,Resource *next_resource, int track);
		int halt();
		bool is_completed();
		int steps_to_destination() const;
		int get_step_delay(int position) const;
		int get_tt_journey_span() const;
		int get_cur_delay() const;
		void clear_cur_delay();

		void perturb(int time);
		std::tuple<int,int> departure_delay();
		void print_timetable(std::ostream& output);
		void print_timetable_delays(std::ostream& output);
		

	    bool operator==(const std::string &train_id);
		friend bool operator<(Train const& train1, Train const& train2);
    	friend std::ostream& operator<< (std::ostream& output, Train const& train);
};

class TrainPriority
{
	double _value;
	public:
		Train *_train;
		void set_value(double value);
		double get_value();
		TrainPriority(Train *train);
		TrainPriority();
		friend bool operator<(TrainPriority const& train1, TrainPriority const& train2);
};

#endif