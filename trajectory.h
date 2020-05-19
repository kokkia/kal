/*
軌跡生成ライブラリ
*/
#ifndef ___KAL_TRAJECTORY_H
#define ___KAL_TRAJECTORY_H
#include <math.h>
#include "config.h"

//type
#define STEP 0
#define LINER 1
#define COSIN 2
#define JERK 3

namespace kal{

template<class T>
class trajectory{
public:
    int type = 0;
    double start_time = 0.0;
    double goal_time;
    double t;//相対時刻
    T start_val;
    T goal_val;
    double interval;
    T L;
    bool start_flag = 0;
    bool goal_flag = 0;
    T a;
    T b;
    T c;
    T d;
    trajectory();
    void creat(T start_val_in,T goal_val_in,double start_time_in,double interval_in,int type_in);
    T get(double now_time);
};

template<class T>
trajectory<T>::trajectory(){
}

template<class T>
void trajectory<T>::creat(T start_val_in,T goal_val_in,double start_time_in,double interval_in,int type_in){
    start_val = start_val_in;
    goal_val = goal_val_in;
    start_time = start_time_in;
    interval = interval_in;
    goal_time = start_time + interval;
    type = type_in;
    L = goal_val - start_val;
    start_flag = true;
    goal_flag = false;
}

template<class T>
T trajectory<T>::get(double now_time){
    T ret_val;
    t = now_time -start_time;
    if(now_time > goal_time){
        goal_flag = true;
        ret_val = goal_val;
    }
    else if(now_time < start_time){
        ret_val = start_val;//@todo:要チェック
    }
    else{
        if(type == STEP){
            ret_val = goal_val;
        }
        else if(type == LINER){
            ret_val = (goal_val-start_val)*t + start_val; 
        }
        else if(type == COSIN){
            ret_val = (goal_val-start_val)/2.0 * (1 -cos(t/interval*PI)) + start_val;
        }
        else if(type == JERK){//jerk min
            double tau = t/interval;
            ret_val = start_val + (goal_val-start_val)*(10.0*tau*tau*tau-15.0*tau*tau*tau*tau+6.0*tau*tau*tau*tau*tau);
        }
    }
    return ret_val;
}

}
#endif