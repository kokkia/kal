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
        double Kdd;
        //ff_param
        double J;
        double D;
        double K;
        //@todo:friction model 入れたい
        
        //関節角度
        RobotData ref_bfr;//qのみ使用
        RobotData state_bfr;

        //出力
        double output;

        void set_fb_param(double Kp,double Ki,double Kd);
        void set_ff_param(double J,double D,double K);

        double feedforward_control(RobotData ref,RobotData state);
        //double feedback_control(RobotData ref,RobotData state);
        double position_control(RobotData ref,RobotData state);
        double velocity_control(RobotData ref,RobotData state);
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

double joint_controller::feedforward_control(RobotData ref,RobotData state){
}

//double joint_controller::feedback_control(RobotData ref,RobotData state){
//}

double joint_controller::position_control(RobotData ref,RobotData state){
    output = Kp * (ref.q - state.q) + Kd * (ref.dq - state.dq);
}

double joint_controller::velocity_control(RobotData ref,RobotData state){
}


#endif