#include "../include/SolutionRg.h"
#include "../include/Train.h"
#include <iostream>


SolutionRg::SolutionRg():Solution()
{
}

int SolutionRg::policy(int train_id){
    int i=1;
    ROccupancy ocp;
    while(true){
        ocp = _network->get_train_nth_resources_occupancy(train_id,i);

        if(ocp.capacity == -1)
            return 0;

        if(ocp.capacity !=1)
            break;
        else if(ocp.up+ocp.down+ocp.transitional == 1)
            return 0;
        
        i++;
    }
    
    if(ocp.capacity == ocp.up+ocp.down+ocp.transitional || ocp.capacity == ocp.down + 1)
        return 0;
    return 1;
}