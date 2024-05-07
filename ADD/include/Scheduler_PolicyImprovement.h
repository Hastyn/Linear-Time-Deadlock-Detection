#ifndef SCHEDULER_POLICY_IMPROVEMENT_H
#define SCHEDULER__POLICY_IMPROVEMENT_H

#include "Scheduler.h"

class Scheduler_PolicyImprovement:public Scheduler
{
	public:	
		Scheduler_PolicyImprovement(Network &network);
		virtual bool run(Solution &solution);
};

#endif