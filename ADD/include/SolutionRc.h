#ifndef SolutionRc_H
#define SolutionRc_H

#include "Solution.h"

class SolutionRc: public Solution
{
	public:	
		SolutionRc();
		int policy(int train_id);
};

#endif