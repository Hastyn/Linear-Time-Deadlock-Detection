#include "../include/TrainEvent.h"


TrainEvent::TrainEvent(int id, int time):_train_id(id),_time(time)
{
}

void TrainEvent::set(int id, int time)
{
    _train_id=id;
    _time=time;
}

void TrainEvent::set_time(int time)
{
    _time = time;
}

void TrainEvent::reset()
{
    set(-1,-1);
}

bool operator<(TrainEvent const& event1, TrainEvent const& event2)
{
    return event2._time<=event1._time;
}

bool operator==(TrainEvent const& event1, int const& event2)
{
    return event1._train_id==event2;
}