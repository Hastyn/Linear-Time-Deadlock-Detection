#ifndef SolutionRG_H
#define SolutionRG_H

#include "Solution.h"

class SolutionRg: public Solution
{
	public:	
		SolutionRg();
		int policy(int train_id);
};

#endif