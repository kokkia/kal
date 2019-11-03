//joint_controller
#ifndef ___JOINT_CONTROLLER_H
#define ___JOINT_CONTROLLER_H
#include "config.h"
#include "utilize.h"

namespace kal{
class joint_controller{
    public:
        //fb_param
        double Kpp;
        double Kpi;
        double Kpd;
        double Kpdd = 0.0;//基本使わない
        double Kvp;
        double Kvi;
        double Kvd;
        //cascade
        double Kcp;
        double Kcd;
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
        double integral_v = 0.0; 

        //出力
        double output;

        void set_fb_param(double Kp,double Ki,double Kd);
        void set_fb_v_param(double Kvp,double Kvi,double Kvd);
        void set_fb_cc_param(double Kcp,double Kcd);
        void set_ff_param(double J,double D,double K);

        double feedforward_control(void);
        double position_control(void);
        double velocity_control(void);
        double cascade_control(void);
        double two_dof_control(void);

};

void joint_controller::set_fb_param(double Kpp,double Kpi,double Kpd){
    this->Kpp = Kpp;
    this->Kpi = Kpi;
    this->Kpd = Kpd;
}

void joint_controller::set_fb_v_param(double Kvp,double Kvi,double Kvd){
    this->Kvp = Kvp;
    this->Kvi = Kvi;
    this->Kvd = Kvd;
}

void joint_controller::set_fb_cc_param(double Kcp,double Kcd){
    this->Kcp = Kcp;
    this->Kcd = Kcd;
}

void joint_controller::set_ff_param(double J,double D,double K){
    this->J = J;
    this->D = D;
    this->K = K;
}

double joint_controller::feedforward_control(void){
    output = J * ref.d2q + D * ref.dq + K * ref.q;
    return output;
}

double joint_controller::position_control(void){
    output = Kpp * (ref.q - state.q) + Kpd * (ref.dq - state.dq) + Kpi * integral;
    integral += (ref.q - state.q) * Ts;//@todo: ワインドアップ対策してない
    return output;
}

double joint_controller::velocity_control(void){
    output = Kvi * integral_v + Kvp * (ref.dq - state.dq) + Kvd * (ref.d2q - state.d2q);
    //@todo:ここでintegralした方がよいのか検討
    integral_v += (ref.dq - state.dq) * Ts;//@todo: ワインドアップ対策してない
    return output;
}

double joint_controller::cascade_control(void){
    ref.dq = Kcp * (ref.q - state.q) + Kcd * (ref.dq - state.dq);
    output = velocity_control();
    return output;
}

double joint_controller::two_dof_control(void){
    output = feedforward_control() + position_control();
    return output;
}

}
#endif