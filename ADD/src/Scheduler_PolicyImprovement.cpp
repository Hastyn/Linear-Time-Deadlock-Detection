#include "../include/Scheduler_PolicyImprovement.h"
#include "../include/SolutionR0.h"
#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

Scheduler_PolicyImprovement::Scheduler_PolicyImprovement(Network &network):Scheduler(network)
{
}

bool Scheduler_PolicyImprovement::run(Solution &solution)
{
    solution.set_network(&(_network));
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
        while(!_current_trains.empty() && !_deadlock)
        {
            TrainPriority tp;
            if(getpid()==parent_pid){
                int best_i = -1;
                float best_pwdd = INT_MAX;
                for(int i=0;i<_current_trains.size();i++)
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

                        // std::cout << "Parent received value from child: " << parentValue << std::endl;

                        close(pipefd[0]); // Close the read end in the parent
                        if(parentValue<best_pwdd){
                            best_i=i;
                            best_pwdd=parentValue;
                        }
                        // Wait for the child process to finish
                        wait(nullptr);
                    }
                    else{
                        tp = _current_trains.at(i);
                        _current_trains.erase(_current_trains.begin()+i);
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
                            tp = _current_trains.at(best_i);
                            _current_trains.erase(_current_trains.begin()+best_i);
                        }
                    }
                    else{
                        if(parent_pwdd>0){
                            if(parent_pwdd<best_pwdd){
                                tp = get_next_current_train();
                            }
                            else{
                                tp = _current_trains.at(best_i);
                                _current_trains.erase(_current_trains.begin()+best_i);
                                improvement = true;
                                _solution = &solution;
                            }
                        }
                        else {   
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

                                // std::cout << "Parent received value from child: " << parentValue << std::endl;

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
                //   std::cout<<"trainid "<<policy<<" "<<tp._train->get_name()<<" "<<moved<<std::endl;
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