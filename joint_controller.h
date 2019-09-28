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
        //ff_param
        double J;
        double D;
        double K;
        //@todo:friction model 入れたい

        RobotData ref_bfr;
        RobotData state_bfr;

        void set_fb_param(double Kp,double Ki,double Kd);
        void set_ff_param(double J,double D,double K);

        double feedback_control(RobotData ref,RobotData state);
        double feedforward_control(RobotData ref,RobotData state);
        double two_DOF_control(RobotData ref,RobotData state);

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

double joint_controller::feedback_control(RobotData ref,RobotData state){
}

#endif