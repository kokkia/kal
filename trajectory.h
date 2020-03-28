/*
軌跡生成ライブラリ
未チェック
*/
#ifndef ___KAL_TRAJECTORY_H
#define ___KAL_TRAJECTORY_H
#include <math.h>
#include "config.h"

//type
#define STEP 0
#define LINER 1
#define COSIN 2

namespace kal{

template<class T>
class trajectory{
public:
    int type = 0;
    double start_time = 0.0;
    double goal_time;
    double t;//現在時刻
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
    if(type == STEP){
        a = goal_val;
        b = start_val;
    }
    else if(type == LINER){//y=ax+b
        b = start_val;
        a = L/interval;
    }
    else if(type == COSIN){//y=acos()+b
        a = L;
        b = start_val;
    }   
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
        if(type == LINER){
            ret_val = a*t + b; 
        }
        else if(type == COSIN){
            ret_val = a *cos(2.0*PI/(interval*2.0) * t) + b;
        }
    }
    return ret_val;
}

}
#endif