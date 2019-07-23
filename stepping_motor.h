#ifndef ___STEPPING_MOTOR_H
#define ___STEPPING_MOTOR_H
#include "config.h"

namespace kal{

class stepping_motor{
    public:
    double n;//分解能P/R
    double v;//velocity[rad/s]
    double x;//position[rad]
    bool dir = 1;//回転方向1:forward,0:backward
    stepping_motor(double n_in);
    void set_direction(bool dir_in);
    double velocity_control(double v);
    double position_control_time(double x);

};

stepping_motor::stepping_motor(double n_in){
    n = n_in;
}

void stepping_motor::set_direction(bool dir_in){
    dir = dir_in;
}

/*
Input: v, output: fp.
*/
double stepping_motor::velocity_control(double v_in){
    v = v_in;
    return n * v / (2 * PI);
}

double stepping_motor::position_control_time(double x){
    return x/v;
}

}
#endif