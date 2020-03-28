#ifndef ___KAL_STEPPING_MOTOR_H
#define ___KAL_STEPPING_MOTOR_H
#include "config.h"
#include <math.h>

#define R2P 1000.0//分解能P/R

namespace kal{

class stepping_motor{
    public:
    double n = R2P;//分解能P/R
    double v;//velocity[rad/s]
    double x;//position[rad]
    bool dir = 1;//回転方向
    stepping_motor();
    stepping_motor(double n_in);
    void set_direction(bool dir_in);
    double velocity2pulse(double v);
    void velocity_control(double v,double& freq,bool& dir);
    double position_control_time(double x);

};

stepping_motor::stepping_motor(){
}

stepping_motor::stepping_motor(double n_in){
    n = n_in;
}

void stepping_motor::set_direction(bool dir_in){
    dir = dir_in;
}

/*
Input: v, output: fp.
*/
double stepping_motor::velocity2pulse(double v_in){
    v = v_in;
    return n * v / (2 * PI);
}

void stepping_motor::velocity_control(double v/*角速度*/,double& freq,bool& dir){
    if(v>=0){
        dir = 1;
    }
    else{
        dir = 0;
    }
    freq = n * fabs(v) / (2 * PI);
    this->dir = dir;
    this->v = v;
}


double stepping_motor::position_control_time(double x){
    return x/v;
}

}
#endif