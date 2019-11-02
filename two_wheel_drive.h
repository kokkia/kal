//two_wheel_drive.h
//2輪駆動ロボット制御ライブラリ
#ifndef ___TWO_WHEEL_DRIVE_H
#define ___TWO_WHEEL_DRIVE_H
#include "config.h"
#include "utilize.h"
//#include "joint_controller.h"
#include "nxtmotor_for_ESP.h"
#include "filter.h"

#define WHEEL_NUM 2

namespace kal{

class two_wheel_drive{
public:
    RobotData<double> ref;
    RobotData<double> state;
    nxtmotor motor[WHEEL_NUM];

    Diff<double> dq_ref[WHEEL_NUM];

    double R;//wheel半径
    double b;//タイヤ間距離
    double reduction_ratio = 1.0;//減速比 =output/input

    void set_param(double R, double b,double reduction_ratio);//車体情報の設定
    void velocity_control(double left_speed,double right_speed);
    void position_control(double distance);
    void odmetry_update();//自己位置の更新
    void line_trace(double left_val,double right_val);
    void direction_control(double forward_speed,double ref_theta);
};

void two_wheel_drive::set_param(double R, double b,double reduction_ratio){
    this->R = R;
    this->b = b;
    this->reduction_ratio = reduction_ratio;
}

void two_wheel_drive::velocity_control(double left_speed,double right_speed){//mm/s
    motor[0].ref.dq = left_speed/R/reduction_ratio;
    motor[1].ref.dq = right_speed/R/reduction_ratio;
    motor[0].drive(motor[0].velocity_control());
    motor[1].drive(motor[1].velocity_control());
}

void two_wheel_drive::position_control(double distance){
    for(int i=0;i<WHEEL_NUM;i++){
        motor[i].ref.q = distance/R/reduction_ratio;
        dq_ref[i].update(motor[i].ref.q, motor[i].ref.dq);
        motor[i].drive(motor[i].position_control());  
    }
}

void two_wheel_drive::line_trace(double left_val,double right_val){

}

void two_wheel_drive::odmetry_update(){//1軸
    //state.q更新後に使う
    state.x = motor[0].state.q * reduction_ratio * R;
}

void two_wheel_drive::direction_control(double forward_speed,double ref_theta){

}

}
#endif