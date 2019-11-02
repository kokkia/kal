//two_wheel_drive.h
//未完成
#ifndef ___TWO_WHEEL_DRIVE_H
#define ___TWO_WHEEL_DRIVE_H
#include "config.h"
#include "utilize.h"
//#include "joint_controller.h"
#include "nxtmotor_for_ESP.h"

namespace kal{

class two_wheel_drive{
public:
    RobotData<double> ref;
    RobotData<double> state;
    nxtmotor motor[2];

    double R;//wheel半径
    double b;//タイヤ間距離
    double reduction_ratio = 1.0;//output/input

    void set_param(double R, double b);
    //void odmetry_update();
    void line_trace(double left_val,double right_val);
    void velocity_control(double left_speed,double right_speed);
    void position_control(double distance);
};

void two_wheel_drive::set_param(double R, double b,double reduction_ratio){
    this->R = R;
    this->b = b;
    this->reduction_ratio = reduction_ratio;
}

void two_wheel_drive::velocity_control(double left_speed,double right_speed){
    motor[0].ref.q = left_speed/R/reduction_ratio;
    motor[1].ref.q = right_speed/R/reduction_ratio;
    motor[0].drive(motor[0].velocity_control());
    motor[1].drive(motor[1].velocity_control());
}

void two_wheel_drive::position_control(double distance){
    motor[0].ref.dq = distance/R/reduction_ratio;
    motor[1].ref.dq = distance/R/reduction_ratio;
    motor[0].drive(motor[0].cascade_control());
    motor[1].drive(motor[1].cascade_control());
}

void two_wheel_drive::line_trace(double left_val,double right_val){

}

}
#endif