#include "../include/Solution.h"
#include<iostream>

Solution::Solution()
{
}


void Solution::set_network(Network const * network)
{
    _network = network;
}

int Solution::policy(int train_id){
    return true;
}

bool Solution::policy_ack(int train_id, int cur_timestep, bool policy, bool moved){
    return true;
}

void Solution::policy_value(int train_id, double *value){
    *value  = 0;
}

void Solution::complete(bool completed, bool deadlock){
}