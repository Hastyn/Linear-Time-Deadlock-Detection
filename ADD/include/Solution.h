#ifndef SOLUTION_H
#define SOLUTION_H

#include<string>
#include <queue>
#include "Network.h"

class Solution
{
    protected:
	Network const*  _network;
	
	public:	
		Solution();
        virtual void set_network(Network const * network);
		virtual int policy(int train_id);	
		virtual bool policy_ack(int train_id, int cur_timestep, bool policy, bool moved);
		virtual void policy_value(int train_id, double *value);
		virtual void complete(bool completed, bool deadlock);
};

#endif