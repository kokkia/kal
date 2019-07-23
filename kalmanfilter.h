//kalman filter for SISO system
//@todo:MIMOに対応したい
//@todo:EKFにも対応させる
#ifndef ___KALMAN_FILTER_H
#define ___KALMAN_FILTER_H
#include "config.h"
#include "utilize.h"
#ifdef EIGEN_KAL
#include "eigen3.3.7/Eigen/Core"
#endif

namespace kal{

template <class Ta,class Tb,class Tc,class Td>
class StateSpace{
  public:
  Ta A;
  Tb B;
  Tc C;
  Td D;

  StateSpace();
  StateSpace(Ta A,Tb B,Tc C,Td D);
  void set_eq(Ta A,Tb B,Tc C,Td D);
};

template <class Ta,class Tb,class Tc,class Td>
StateSpace<Ta,Tb,Tc,Td>::StateSpace(){
}

template <class Ta,class Tb,class Tc,class Td>
StateSpace<Ta,Tb,Tc,Td>::StateSpace(Ta A,Tb B,Tc C,Td D){
  this->A = A;
  this->B = B;
  this->C = C;
  this->D = D;
}

template <class Ta,class Tb,class Tc,class Td>
void StateSpace<Ta,Tb,Tc,Td>::set_eq(Ta A,Tb B,Tc C,Td D){
  this->A = A;
  this->B = B;
  this->C = C;
  this->D = D;
}

//Kalmanfilter for SISO
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
  StateSpace<T,T,T,T> sys;
  KalmanFilter();
  KalmanFilter(T xx,T QQ, T RR);
  void update(T u,T y);
};

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

#ifdef EIGEN_KAL
//@todo:継承する？
//Kalmanfilter for MIMO
template <class Ta,class Tb,class Tc,class Td,class Tx, class Ty>
class KalmanFilter_MIMO{
 
public:
  Tx x_;
  Tx x_est;
  Ty y;
  Eigen::MatrixXd G;
  Ta P_bfr;//事前
  Ta P_aft;//事後
  Eigen::MatrixXd Q;//vの分散
  Eigen::MatrixXd R;//wの分散
  StateSpace<Ta,Tb,Tc,Td> sys;
  KalmanFilter_MIMO();
  KalmanFilter_MIMO(Tx x, Eigen::MatrixXd Q, Eigen::MatrixXd R);
  KalmanFilter_MIMO(StateSpace<Ta,Tb,Tc,Td> sys_in,Tx x, Eigen::MatrixXd Q, Eigen::MatrixXd R);
  void update(Tx u,Ty y,Tx& x_ret);
};

template <class Ta,class Tb,class Tc,class Td,class Tx, class Ty>
KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Ty>::KalmanFilter_MIMO(){
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Ty>
KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Ty>::KalmanFilter_MIMO(Tx x,Eigen::MatrixXd Q, Eigen::MatrixXd R){
  this->x_est = x;
  this->Q = Q;
  this->R = R;
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Ty>
KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Ty>::KalmanFilter_MIMO(StateSpace<Ta,Tb,Tc,Td> sys_in,Tx x,Eigen::MatrixXd Q, Eigen::MatrixXd R): sys(sys_in){
  this->x_est = x;
  this->Q = Q;
  this->R = R;
}

template <class Ta,class Tb,class Tc,class Td,class Tx, class Ty>
void KalmanFilter_MIMO<Ta,Tb,Tc,Td,Tx,Ty>::update(Tx u,Ty y,Tx& x_ret){
  x_ = sys.A * x_est + sys.B *u;
  P_bfr = sys.A * P_aft * sys.A.transpose() + sys.B * Q * sys.B.transpose();
  G = P_bfr * sys.C.transpose() * (sys.C * P_bfr * sys.C.transpose() + R).inverse();
  x_est = x_ + G * (y - sys.C * x_);
  x_ret = x_est;
  P_aft = (Eigen::MatrixXd::Identity(sys.A.rows(),sys.A.cols()) - G * sys.C) * P_bfr;
}

#endif

}

#endif
