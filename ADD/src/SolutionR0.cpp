#include "../include/SolutionR0.h"
#include "../include/Train.h"
#include<iostream>


SolutionR0::SolutionR0():Solution()
{
}

int SolutionR0::policy(int train_id){
    // Passing Node Algorithm
    int i=1;
    ROccupancy ocp,ocp2,ocp3,ocp4,ocp5;
    ocp3 = _network->get_train_nth_resources_occupancy(train_id,1);
    ocp2= _network->get_train_nth_resources_occupancy(train_id,0);
    ocp5=_network->get_train_nth_resources_occupancy(train_id,3);
    int p=0;
    int action=0;

    while(true){
        // Skipping single track nodes as we will not stop a train on these
        ocp = _network->get_train_nth_resources_occupancy(train_id,i);
        if(ocp.capacity==-1 || ocp.capacity - ocp.up - ocp.down - ocp.transitional == 0) return 0;
        else if(ocp.capacity==1 && ocp.capacity - ocp.up - ocp.down - ocp.transitional > 0){
            i++;
            continue;
        }
        else if(ocp.capacity>1 && ocp.capacity - ocp.up - ocp.down - ocp.transitional > 0) break;
        else if(ocp.capacity>20) break;
        else{
            return 0;
        }
    }

    action = i;
    if(ocp.up>=1 || ocp.capacity-ocp.up-ocp.down-ocp.transitional>=2) return i;
    i++;
    while(true){
        // Checking if a there is a path to a red node from the black node formed by the action of moving the given train
        ocp = _network->get_train_nth_resources_occupancy(train_id,i);
        if(ocp.capacity>1)
        {
            if(ocp.capacity-ocp.up-ocp.down-ocp.transitional>=1) return action;
            else if(ocp.capacity==ocp.up) return 0;
            else if(ocp.capacity>20) return action;
        }
        else if((ocp.capacity==1 & ocp.capacity-ocp.up-ocp.down-ocp.transitional==0) || ocp.capacity==-1) return 0;
        i++;
    }
    return 0;
}
