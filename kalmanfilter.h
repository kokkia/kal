//kalman filter for SISO system
//@todo:MIMOに対応したい
//@todo:EKFにも対応させる
#ifndef ___KALMAN_FILTER_H
#define ___KALMAN_FILTER_H
#include "config.h"
#include "utilize.h"

namespace kal{

template <class T>
class StateSpace{
  T A;
  T B;
  T C;
  T D;
public:
  void set_eq(T A,T B,T C,T D);
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


template <class T>
void StateSpace<T>::set_eq(T A,T B,T C,T D){
  
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
