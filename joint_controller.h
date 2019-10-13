//joint_controller
//構成検討中，未実装
#ifndef ___JOINT_CONTROLLER_H
#define ___JOINT_CONTROLLER_H
#include "config.h"
#include "utilize.h"

namespace kal{
class joint_controller{
    public:
        //fb_param
        double Kp;
        double Ki;
        double Kd;
        double Kdd = 0.0;//基本使わない
        //ff_param
        double J;
        double D;
        double K;
        //@todo:friction model 入れたい
        
        //関節角度
        RobotData<double> ref;//qのみ使用
        RobotData<double> state;
        RobotData<double> ref_bfr;//qのみ使用
        RobotData<double> state_bfr;
        double integral = 0.0;

        //出力
        double output;

        void set_fb_param(double Kp,double Ki,double Kd);
        void set_ff_param(double J,double D,double K);

        double feedforward_control(void);
        //double feedback_control(RobotData ref,RobotData state);
        double position_control(void);
        double velocity_control(void);
        //double two_DOF_control(RobotData ref,RobotData state);

};

void joint_controller::set_fb_param(double Kp,double Ki,double Kd){
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

void joint_controller::set_ff_param(double J,double D,double K){
    this->J = J;
    this->D = D;
    this->K = K;
}

double joint_controller::feedforward_control(){
    output = J * ref.d2q + D * ref.dq + K * ref.q;
    return output;
}

//double joint_controller::feedback_control(RobotData ref,RobotData state){
//}

double joint_controller::position_control(){
    output = Kp * (ref.q - state.q) + Kd * (ref.dq - state.dq) + Ki * integral;
    integral += (ref.q - state.q) * Ts;
    return output;
}

double joint_controller::velocity_control(){
    output = Kp * (ref.q - state.q) + Kd * (ref.dq - state.dq) + Kdd * (ref.d2q - state.d2q);
    //@todo:ここでintegralした方がよいのか検討
    return output;
}


#endif