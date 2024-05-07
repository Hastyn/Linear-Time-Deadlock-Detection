#ifndef SolutionR0_H
#define SolutionR0_H

#include "Solution.h"

class SolutionR0: public Solution
{
	public:	
		SolutionR0();
		int policy(int train_id);
};

#endif