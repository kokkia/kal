//インピーダンス制御用
//@todo:インピーダンス型の実装
//未チェック
#ifndef ___KAL_IMPEDANCE_CONTROL
#define ___KAL_IMPEDANCE_CONTROL

#include "config.h"
#include "utilize.h"
#ifdef EIGEN_KAL
#include "eigen3.3.7/Eigen/Core"
#endif

//#define IMPEDANCE_FORM
#define ADMITANCE_FORM

namespace kal{

template<class T,class Tmat>
class impedance_control{
public:
    RobotData<T> zero;//基準位置   
    //desired imoedance
    Tmat Md;//N/m/s^2=kg
    Tmat Cd;//N/m/s
    Tmat Kd;//N/m
    //admitance control
    RobotData<T> ref;
    RobotData<T> ref_b1;
    RobotData<T> ref_b2;
    T fext;
    T fext_b1;
    T fext_b2;
    
    impedance_control();
    impedance_control(Tmat Md,Tmat Cd,Tmat Kd);
    void set_impedance(Tmat Md,Tmat Cd,Tmat Kd);
    void impedance_out(RobotData<T> state,T fref,T& f); //運動入力力出力  
    void admitance_out(T fext,T fref,RobotData<T>& ref);//力入力運動出力
};

template<class T,class Tmat>
impedance_control<T,Tmat>::impedance_control(){
}

template<class T,class Tmat>
impedance_control<T,Tmat>::impedance_control(Tmat Md,Tmat Cd,Tmat Kd){
    this->Md = Md;
    this->Cd = Cd;
    this->Kd = Kd;
}

template<class T,class Tmat>
void impedance_control<T,Tmat>::set_impedance(Tmat Md,Tmat Cd,Tmat Kd){
    this->Md = Md;
    this->Cd = Cd;
    this->Kd = Kd;
}

template<class T,class Tmat>
void impedance_control<T,Tmat>::impedance_out(RobotData<T> state,T fref,T& f){
    f = Md * state.d2x + Cd * state.dx + Kd * state.x;//@todo:動力学補償入れる
}

template<class T,class Tmat>
void impedance_control<T,Tmat>::admitance_out(T fext,T fref,RobotData<T>& ref){//速度出力
    this->fext = fext;
    this->ref.dx = 1.0/(4*Md+2*Ts*Cd+Kd*Ts*Ts)*(2*Ts*(fext-this->fext_b2)-(2*Kd*Ts*Ts-8*Md)*this->ref_b1.dx-(4*Md-2*Ts*Cd+Kd*Ts*Ts)*this->ref_b2.dx);
    
    this->fext_b2 = this->fext_b1;
    this->fext_b1 = fext;
    this->ref_b2 = this->ref_b1;
    this->ref_b1 = this->ref;
    ref.dx = this->ref.dx*1000.0;//mm/sに直す
}

}


#endif