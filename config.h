//robot configulation
#ifndef ___KAL_CONFIG_H
#define ___KAL_CONFIG_H

//使うコントローラを選択
//#define ARDUINO_KAL
// #define ESP32
#define UBUNTU 
//#define EIGEN_KAL
//#define X2_KAL

//Sampling Time
#ifdef ARDUINO_KAL
// #define Ts 0.002//sampling time[ms]
#elif defined ESP32
#define Ts 0.01
// #define Ts 0.005
#elif defined UBUNTU
#define Ts 0.01
#endif

#ifndef X2_KAL
#define PI 3.14159265359
#endif

#define DOF 1

namespace kal{

template<class T>
class RobotData{
public:
  //姿勢
  T phi;
  T dphi;
  T d2phi;
  T theta;
  T dtheta;
  T d2theta;
  //位置
  T x;
  T dx;
  T d2x;
  T y;
  T dy;
  T d2y;
  //関節用
  T q;
  T dq;
  T d2q;
  
  RobotData();  
  
};

template<class T>
RobotData<T>::RobotData(){
  phi = 0.0;
  dphi = 0.0;
  d2phi = 0.0;
  theta = 0.0;
  dtheta = 0.0;
  d2theta = 0.0;
  x = 0.0;
  dx = 0.0;
  d2x = 0.0;
  y = 0.0;
  dy = 0.0;
  d2y = 0.0;
  q = 0.0;
  dq = 0.0;
  d2q = 0.0;
}


class q_data{
  public:
    double q[DOF];
    double dq[DOF];
    double d2q[DOF];
    q_data();
};

q_data::q_data(){
  for(int i = 0;i<DOF;i++){
    q[i] = 0.0;
    dq[i] = 0.0;
    d2q[i] = 0.0;
  }
}

}

#endif
