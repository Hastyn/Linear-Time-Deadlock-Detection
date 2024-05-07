#ifndef JOURNEY_H
#define JOURNEY_H

#include<string>

typedef struct Journey_Resource
{
    int _res;
    int _res_type;
    int _track;
    bool _direction_down;
}JResource;

class Journey
{
    int _min_time;
    int _tt_dep_time;
    int _track;
    int _scheduled_dep_time;
    bool _direction_down;

	public:
    int _res;
    int _res_type;
    
    Journey(int res, int res_type, int min_time, int tt_dep_time, bool _direction_down);
    int get_min_time() const;
    int get_tt_dep_time() const;
    int get_tt_dep_constraint() const;
    bool set_scheduled_dep_time(int time);
    int get_scheduled_dep_time() const;

    bool set_track(int track);
    int get_track() const;

    bool get_direction() const;
    bool set_direction(bool direction_down);

    JResource get_resource() const;
	bool perturb(int time);

    friend std::ostream& operator<< (std::ostream& output, Journey const& journey);

};

#endif