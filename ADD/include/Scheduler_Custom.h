#ifndef SCHEDULER_CUSTOM_H
#define SCHEDULER_CUSTOM_H

#include "Scheduler.h"

class Scheduler_Custom:public Scheduler
{
	protected:
		bool _completed;
		bool _deadlock;
		Network& _network;
		std::priority_queue<TrainEvent> _pq;
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
		Scheduler_Custom(Network &network);
		virtual bool run(Solution &solution);
		void save_timetable(std::string filename);
		float get_pwdd();
};

#endif