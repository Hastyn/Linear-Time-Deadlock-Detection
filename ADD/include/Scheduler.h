#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<string>
#include <queue>
#include <list> 
#include "Solution.h"
#include "Network.h"
#include "Train.h"

class Scheduler
{
	protected:
		bool _completed;
		bool _deadlock;
		std::priority_queue<TrainEvent> _pq;
		Network &_network;
		Solution *_solution;
		std::vector<int> _moved_trains;
		int _cur_timestep;
		std::vector<TrainPriority> _current_trains;
		void release_moved_trains();
		virtual void initialize_pq();

		virtual int get_current_trains();
		virtual TrainPriority get_next_current_train();
		virtual bool on_move(Train *train);
		virtual bool on_halt(Train *train);
		virtual bool getPolicyAction(Solution &solution, TrainPriority &tp);
		virtual void sendPolicyAck(Solution &solution, int train_id, bool policy, bool moved);
	public:	
		Scheduler(Network &network);
		virtual bool run(Solution &solution);
		void save_timetable(std::string filename);
		void compare_delays();
		float get_pwdd();
};

#endif