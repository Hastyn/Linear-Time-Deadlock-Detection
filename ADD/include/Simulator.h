#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include "Solution.h"
#include "Network.h"

typedef struct SmConfig{
    int num_stations;   
    int num_sections;
    int num_trains;
    int deadlock_patience;
    std::string stations_file;
    std::string sections_file;
    std::string timetable_file;
    time_t start_time;
}SimulatorConfig;

class Simulator
{
	private:
        Network _base_network;
        int num_stations;   
        int num_sections;
        int num_trains;
        time_t start_time;
	public:
        Simulator(std::string data_path, std::string config_file, std::string timetable_file );
        Simulator(std::string data_path, std::string config_file, std::string timetable_file ,bool custom);
        std::tuple<bool,float> run(Solution &solution, int scheduler_type, int seed, int max_perturb, float threshold = 0.0);
        void printStationNames(Network &network);

};

#endif