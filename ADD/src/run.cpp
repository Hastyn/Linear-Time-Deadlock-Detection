#include<iostream>
#include <chrono> 
#include<string>
#include <vector>
#include <numeric>
#include <cmath>
#include "../include/Simulator.h"
#include "../include/SolutionRg.h"
#include "../include/SolutionR0.h"
#include "../include/SolutionRc.h"

#define FILE_DIR std::string("../data/")
#define PERTURB_FOLDER std::string("perturbed_timetables/")

int iterations=10;
int success_count=0;
double pwdd_total=0; // Since the priority is 1, PWDD (Priority Weighted Departure Delay) is equivalento to ADD (Average Departure Delay)
std::string algo = "Rg";
std::string policy_improv="false";
std::string optimize="false";
int seed = -1;
int max_perturb = -1;
std::string line = "HYP-1";
std::string policyfile = "weights.txt";
int state_type=0;
int total_duration=0;
bool preprocess = true;
int scheduler_type = 0;
float policy_threshold=0.0;

double calculate_mean(const std::vector<double>& data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double calculate_standard_deviation(const std::vector<double>& data) {
    double mean = calculate_mean(data);

    double sum_squared_diff = 0.0;
    for (const auto& value : data) {
        double diff = value - mean;
        sum_squared_diff += diff * diff;
    }

    double variance = sum_squared_diff / data.size();
    return std::sqrt(variance);
}

bool simulate(std::string path,std::string config_file, std::string timetable_file)
{
    auto start = std::chrono::high_resolution_clock::now(); 
    bool success;
    float pwdd;

    if(policy_improv.compare("true") == 0 && optimize.compare("true") == 0)
        scheduler_type = 3;
    else if(policy_improv.compare("true") == 0)
        scheduler_type = 2;
    else if(algo.compare("Rg") == 0)
        scheduler_type = 1;
    else 
        scheduler_type = 0;
    
    if(algo.compare("Rg") == 0 ){
        Simulator sim(path,config_file,timetable_file,true);
        SolutionRg sol;
        std::tie(success,pwdd)=sim.run(sol,scheduler_type,seed,max_perturb,policy_threshold);
    }
    else if(algo.compare("R0") == 0){
        Simulator sim(path,config_file,timetable_file);
        SolutionR0 sol;
        std::tie(success,pwdd)=sim.run(sol,scheduler_type,seed,max_perturb,policy_threshold);
    }
    else if(algo.compare("Rc") == 0){
        Simulator sim(path,config_file,timetable_file);
        SolutionRc sol;
        std::tie(success,pwdd)=sim.run(sol,scheduler_type,seed,max_perturb,policy_threshold);
    }
    else{
        Simulator sim(path,config_file,timetable_file);
        Solution sol;
        std::tie(success,pwdd) = sim.run(sol,scheduler_type,seed,max_perturb,policy_threshold);
    }
    
    if(success){
        success_count++;
        pwdd_total+=pwdd;
    }        
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count(); 
    // std::cout<<" "<<success<<" "<<pwdd<<" "<<duration<<std::endl;
    total_duration +=duration;
    return true;
}


int main(int argc, char **argv)
{  
    if(argc>1)
        line=argv[1];
    if(argc>2){
        algo = argv[2];
    }
    if(argc>3)
    {
        policy_improv = argv[3];
    }
    if(argc>4){
        optimize = argv[4];
    }
    if(argc>5)
    {
        iterations = std::stoi(argv[5]);
    }


    std::string path = FILE_DIR+line+"/";
    std::string config_file = path+line+".cnf";
    std::string timetable_path="";
    std::vector<double> pwdds;

    auto start = std::chrono::high_resolution_clock::now(); 
    double pwdd_pre=0;
    if(iterations ==1)
        simulate(path,config_file,timetable_path);
    else
    {    
        timetable_path = PERTURB_FOLDER+line+"-Timetable-";
        for(int i=0;i<iterations;i++)
        {   
            if(!simulate(path,config_file,timetable_path+std::to_string(i+1)+".csv"))
                return 0;
            else{    
                pwdds.push_back(pwdd_total-pwdd_pre);
                pwdd_pre = pwdd_total;
            }
        }
    }
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count(); 

    if(iterations-success_count>0){
        std::cout<<line<<" "<<algo<<" deadlock"<<std::endl;
    }
    else{
        std::cout<<line<<" "<<algo<<", ADD : "<<pwdd_total/iterations<<", Standard Error : "<<calculate_standard_deviation(pwdds)/std::sqrt(iterations)<<std::endl;
    }
    return 0;
}