#ifndef SCHEDULER_POLICY_IMPROVEMENT_H
#define SCHEDULER__POLICY_IMPROVEMENT_H

#include "Scheduler.h"

class Scheduler_OptimizedPolicyImprovement:public Scheduler
{
	public:	
		Scheduler_OptimizedPolicyImprovement(Network &network);
		virtual bool run(Solution &solution);
        virtual bool on_halt(Train *train);
};

#endif