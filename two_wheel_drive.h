//two_wheel_drive.h
//未完成
#ifndef ___TWO_WHEEL_DRIVE_H
#define ___TWO_WHEEL_DRIVE_H
#include "config.h"
#include "utilize.h"
#include "joint_controller.h"

namespace kal{

class two_wheel_drive{
public:
    RobotData<double> ref;
    RobotData<double> state;
    joint_controller motor[2];

    double R;//wheel半径
    double b;//タイヤ間距離

    void set_param(double R, double b);
    //void odmetry_update();
    //void line_trace();
    void velocity_control(double left_speed,double right_speed);
};

void two_wheel_drive::set_param(double R, double b){
    this->R = R;
    this->b = b;
}

void two_wheel_drive::velocity_control(double left_speed,double right_speed){
    motor[0].ref.dq = left_speed;
    motor[1].ref.dq = right_speed;
    motor[0].velocity_control();
}

}
#endif