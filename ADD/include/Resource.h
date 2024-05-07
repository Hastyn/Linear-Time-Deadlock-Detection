#ifndef RESOURCE_H
#define RESOURCE_H

#include<string>
#include<vector>
#include "../include/TrainEvent.h"

typedef struct Resource_Occupancy{
	int capacity;
	int up;
	int down;
	int transitional;
}ROccupancy;

class Resource
{
	protected:
		int _id;
		std::string _name;
		int _capacity;
		int _state;
		TrainEvent _next_event;
		std::vector<TrainEvent> _tracks;
		int _occupied_up;
		int _occupied_down;
		int _occupied_transitional;
        int setState(int state);
	
	public:
		Resource(int id,std::string name, int capacity);
		int get_id() const;
		std::string get_name() const;
		int get_num_free_tracks() const;
		int get_next_event_time() const;
		void update_next_event();
		int check_occupancy(int train_id,bool direction_down) ;
		void update_track_event(int track, int time);
		bool initiate_release(int train_id, bool direction_down);
		void release(int train_id, int track);
		int occupy(int train_id, bool direction_down);
		ROccupancy get_occupancy()const;
		bool operator==(const std::string &name);
		
};

#endif