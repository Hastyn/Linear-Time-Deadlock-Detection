#include "../include/SolutionRc.h"
#include "../include/Train.h"
#include<iostream>


SolutionRc::SolutionRc():Solution()
{
}

int SolutionRc::policy(int train_id){
    int i=1;
    ROccupancy ocp,ocp2,ocp3,ocp4;
    ocp4 = _network->get_train_nth_resources_occupancy(train_id,1);
    while(true)
    {
        ocp = _network->get_train_nth_resources_occupancy(train_id,i);
        
        if(ocp.capacity==-1 || ocp.capacity - ocp.up - ocp.down - ocp.transitional == 0)
        {
            return 0;
        }
        
        if(ocp.capacity!=1 ) 
        {
            if(ocp.capacity - (ocp.up+ocp.down+ocp.transitional)>=1 && false)
            {
             ///std::cout<<"got into loop yayy"<<std::endl;
                ocp2 = _network->get_train_nth_resources_occupancy(train_id,i+2);
                ocp3 = _network->get_train_nth_resources_occupancy(train_id,i+1);
                if(ocp2.down>=1 && ocp3.capacity-(ocp3.up+ocp2.down+ocp3.transitional)>=1 &&  ocp4.capacity>1)
                    return 1;
                if(ocp2.down>=1 && ocp3.capacity-(ocp3.up+ocp2.down+ocp3.transitional)>=1 &&  ocp4.capacity==1)
                    return 2;
                if(ocp2.down>=1 && ocp3.capacity-(ocp3.up+ocp2.down+ocp3.transitional)>=1)
                    return true;
            } 
            if(ocp.capacity - ocp.up - ocp.down - ocp.transitional == 1)
            {
                i++;
                continue;
            }
            if(ocp.capacity - ocp.up - ocp.down - ocp.transitional >= 2 )
            {
                return i;  
            }
            else
                break;  
        }
        
        i++;
    }
    
    if(ocp.capacity == ocp.up+ocp.down+ocp.transitional || ocp.capacity == ocp.down + 1 )
    {
        // This is the case criticality changes to -1
        return 0;
    }
    
    return 1;
}
