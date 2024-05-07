#include "../include/Scheduler_OptimizedPolicyImprovement.h"
#include "../include/SolutionR0.h"
#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

Scheduler_OptimizedPolicyImprovement::Scheduler_OptimizedPolicyImprovement(Network &network):Scheduler(network)
{
}

bool Scheduler_OptimizedPolicyImprovement::run(Solution &solution)
{
    solution.set_network(&(_network));
    int max_width = 5; // Width of the set of actions explored at each step
    int halt_times = 0,last_timestep=0;
    int parent_pid = getpid();
    int pipefd[2]; // File descriptors for the pipe
    bool improvement = false; // If a policy better than the cur_policy has been found
    SolutionR0 curr_policy;
    curr_policy.set_network(&(_network));
    float parent_pwdd = -1;
    _solution = &curr_policy;

    _completed=false;
    _deadlock =false;
    int cnt =0; // Number of steps of policy improvement
    while(!_completed && !_deadlock){
        last_timestep = _cur_timestep;

        _cur_timestep = get_current_trains();

        if(last_timestep != _cur_timestep && !_moved_trains.empty())
        {
            release_moved_trains();
        }
        
        if(_cur_timestep == -1){
            _cur_timestep=last_timestep;
            if(_pq.size()!=0)
                std::cout<<"Error: PQ is not empty "<<_cur_timestep<<std::endl;
            _completed = true;
            break;
        }
        int curr_size  = _current_trains.size();
        while(!_current_trains.empty() && !_deadlock)
        {
            TrainPriority tp;
            cnt++;
            if(getpid()==parent_pid && cnt<=50){

                int best_i = -1;
                float best_pwdd = INT_MAX;
                std::vector<unsigned int> indices(_current_trains.size());
                std::iota(indices.begin(), indices.end(), 0);
                std::random_shuffle(indices.begin(), indices.end());
                // Selecting max_width number actions to explore uniformly at random

                for(int i=0;i<std::min((int)_current_trains.size(),max_width);i++)
                {
                    pid_t pid;

                    // Create a pipe
                    if (pipe(pipefd) == -1) {
                        perror("pipe");
                        return 1;
                    }

                    // Fork the process to compute rollout delay after ith action
                    pid = fork();

                    if (pid == -1) {
                        perror("fork");
                        return 1;
                    }

                    if(pid!=0) { // Parent process
                        close(pipefd[1]); // Close the write end in the parent

                        // Read the value from the pipe
                        float parentValue;
                        read(pipefd[0], &parentValue, sizeof(parentValue));

                        close(pipefd[0]); // Close the read end in the parent
                        if(parentValue<best_pwdd){
                            best_i=indices[i];
                            best_pwdd=parentValue;
                        }
                        // Wait for the child process to finish
                        wait(nullptr);
                    }
                    else{
                        tp = _current_trains.at(indices[i]);
                        _current_trains.erase(_current_trains.begin()+indices[i]);
                        improvement = true;
                        _solution = &solution;
                        break;
                    }
                }
                if(getpid()==parent_pid)
                {
                    if(improvement)
                    {
                        if(best_i==-1){
                            tp = get_next_current_train();
                        }
                        else{
                            if(parent_pwdd>0)
                            {   
                                if(best_pwdd<parent_pwdd){
                                    // Better policy found
                                    tp = _current_trains.at(best_i);
                                    _current_trains.erase(_current_trains.begin()+best_i);
                                    parent_pwdd = best_pwdd;
                                }
                                else{
                                    // Previous policy was better
                                    tp = get_next_current_train();
                                }
                            }
                            else{
                                tp = get_next_current_train();
                            }
                        }
                    }
                    else{
                        if(parent_pwdd>0){
                            if(parent_pwdd<best_pwdd){
                                tp = get_next_current_train();
                            }
                            else{
                                // Policy better than the curr_policy is found

                                tp = _current_trains.at(best_i);
                                _current_trains.erase(_current_trains.begin()+best_i);
                                improvement = true;
                                _solution = &solution;
                                parent_pwdd = best_pwdd;
                            }
                        }
                        else {   
                            // Compute the pwdd of the curr_policy

                            pid_t pid;

                            // Create a pipe
                            if (pipe(pipefd) == -1) {
                                perror("pipe");
                                return 1;
                            }

                            // Fork the process
                            pid = fork();

                            if (pid == -1) {
                                perror("fork");
                                return 1;
                            }

                            if(pid!=0) { // Parent process
                                close(pipefd[1]); // Close the write end in the parent

                                // Read the value from the pipe
                                float parentValue;
                                read(pipefd[0], &parentValue, sizeof(parentValue));

                                close(pipefd[0]); // Close the read end in the parent
                                parent_pwdd = parentValue;
                                if(parentValue<best_pwdd){
                                    best_pwdd=parentValue;
                                    tp = get_next_current_train();
                                }
                                else{
                                    tp = _current_trains.at(best_i);
                                    _current_trains.erase(_current_trains.begin()+best_i);
                                    improvement = true;
                                    _solution = &solution;
                                }
                                // Wait for the child process to finish
                                wait(nullptr);
                            }
                            else{
                                tp = get_next_current_train();
                            }
                        }
                    }  
                }
            }
            else{
                tp = get_next_current_train();
            }

            if(tp._train == NULL)
                continue;

            int train_id = tp._train->get_id();
            bool moved = false;
            
            if(improvement)
            {
                int policy = getPolicyAction(solution,tp);

                if(policy)
                {
                    moved = on_move(tp._train);
                }
                if(!moved)
                {
                    _deadlock = !on_halt(tp._train);
                }
                sendPolicyAck(solution, train_id, policy, moved);
            }
            else{
                int policy = getPolicyAction(curr_policy,tp);

                if(policy)
                {
                    moved = on_move(tp._train);
                }
                //   std::cout<<"trainid "<<policy<<" "<<tp._train->get_name()<<" "<<moved<<std::endl;
                if(!moved)
                {
                    _deadlock = !on_halt(tp._train);
                }
                sendPolicyAck(curr_policy, train_id, policy, moved);
            }
          
        }
    }

    float rollout_pwdd;
    if(_completed && !_deadlock){
        rollout_pwdd=get_pwdd();
    }
    else{
        rollout_pwdd = INT_MAX;
    }

    if(getpid()!=parent_pid){

        close(pipefd[0]); // Close the read end in the child

        // Write the value to the pipe
        write(pipefd[1], &rollout_pwdd, sizeof(rollout_pwdd));

        close(pipefd[1]); // Close the write end in the child
        exit(0);
    }

    if(improvement)
        solution.complete(_completed,_deadlock);
    else
        curr_policy.complete(_completed,_deadlock);

    return _completed && !_deadlock;
}

bool Scheduler_OptimizedPolicyImprovement::on_halt(Train *train)
{
    bool success = _network.optimized_halt(train->get_id());
    if(success)
        _pq.emplace(train->get_id(),train->get_next_event_time());
    return success;
}