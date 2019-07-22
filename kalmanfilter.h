//kalman filter for SISO system
//@todo:MIMOに対応したい
//@todo:EKFにも対応させる
#ifndef ___KALMAN_FILTER_H
#define ___KALMAN_FILTER_H
#include "config.h"
#include "utilize.h"

namespace kal{

template <class T1,class T2,class T3,class T4>
class StateSpace{
  T1 A;
  T2 B;
  T3 C;
  T4 D;
public:
  void set_eq(T1 A,T2 B,T3 C,T4 D);
};

template <class T>
class KalmanFilter{
 
public:
  T x_;
  T x_est;
  T y;
  T g;
  T P_bfr;
  T P_aft;
  T Q;//vの分散
  T R;//wの分散
//  StateSpace sys;
  KalmanFilter();
  KalmanFilter(T xx,T QQ, T RR);
  void update(T u,T y);
};


template <class T1,class T2,class T3,class T4>
void StateSpace<T1,T2,T3,T4>::set_eq(T1 A,T2 B,T3 C,T4 D){
  this->A  = A;
  this->B  = B;
  this->C  = C;
  this->D  = D;
}
template <class T>
KalmanFilter<T>::KalmanFilter(){
  x_est = 0.0;
  Q = 1.0;
  R = 1.0;
}

template <class T>
KalmanFilter<T>::KalmanFilter(T xx,T QQ, T RR){
  x_est = xx;
  Q = QQ;
  R = RR;
}

template <class T>
void KalmanFilter<T>::update(T u,T y){
  x_ = x_est + Ts * u;
  P_bfr = P_aft + Q;
  g = P_bfr / (P_bfr + R);
  x_est = x_ + g * (y - x_);
  P_aft = (1 - g*1) * P_bfr;
}


}

#endif
