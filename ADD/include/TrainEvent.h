#ifndef TRAINEVENT_H
#define TRAINEVENT_H

class TrainEvent
{
	public:
		int _train_id;
		int _time;
		
		TrainEvent(int id=-1, int time=-1);
		void reset();
		void set(int id, int time=-1);
		void set_time(int time);
		friend bool operator<(TrainEvent const& event1, TrainEvent const& event2);
		friend bool operator==(TrainEvent const& event1, int const& event2);
};


#endif