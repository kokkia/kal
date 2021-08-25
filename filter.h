//filter and differentiator library
#ifndef ___KAL_FILTER_H
#define ___KAL_FILTER_H
#include "config.h"
#include "utilize.h"

#define FC_Diff 100//カットオフ

namespace kal{

//filter base
template <class T>
class filter_base{
public:
  T x;
  T x_bfr;
  T y;
  T y_bfr;
  void update();
  filter_base();
  filter_base(T x_in);    
};

template <class T>
filter_base<T>::filter_base(){
  x=0.0;
  x_bfr=0.0;
  y_bfr=0.0;
}
template <class T>
filter_base<T>::filter_base(T x_in){
  x=x_in;
  x_bfr=x_in;
  y = 0.0;
  y_bfr=0.0;
}

//LPF
template <class T>
class LPF : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  LPF();
  LPF(T x_in, double fc_in);
  void update(T xm,T& ret_xm);
};

template<class T>
LPF<T>::LPF(T x_in, double fc_in) : filter_base<T>(x_in){
  fc = fc_in;
  this->y = x_in;
}


template <class T>
void LPF<T>::update(T xm,T& ret_xm){
  double Tc = 1.0/(2*PI*fc);
  //this->x=Ts/(Ts+2*Tc)*xm +2*Tc/(Ts+2*Tc)*this->x;
  this->y=Ts/(Ts+2*Tc)*xm + Ts/(Ts+2*Tc)*this->x - (Ts-2*Tc)/(Ts+2*Tc)*this->y;  
  this->x = xm;
  ret_xm = this->y;
}


//HPF
template <class T>
class HPF : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  HPF();
  HPF(T x_in, double fc_in);
  void update(T xm,T& ret_xm);
};

template<class T>
HPF<T>::HPF(T x_in, double fc_in) : filter_base<T>(x_in){
  fc = fc_in;
  this->y = x_in;
}

template <class T>
void HPF<T>::update(T xm,T& ret_xm){
  double Tc = 1.0/(2*PI*fc);
  //this->x = 2.0*Tc/(Ts+2.0*Tc)*xm - Ts/(Ts+2.0*Tc)*this->x;
  this->y = 2.0*Tc/(Ts+2.0*Tc)*xm - 2.0*Tc/(Ts+2.0*Tc)*this->x - (Ts-2.0*Tc)/(Ts+2.0*Tc)*this->y;
  this->x = xm;
  ret_xm = this->y;
}

//differentiator
template <class T>
class Diff : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  Diff();
  Diff(T x_in, double fc_in);
  void update(T xm,T& dxm);
};

template<class T>
Diff<T>::Diff() : filter_base<T>(){
  fc = FC_Diff;
}

template<class T>
Diff<T>::Diff(T x_in, double fc_in) : filter_base<T>(x_in){
  fc=fc_in;
}

template <class T>
void Diff<T>::update(T xm/*現在値*/,T& dxm/*微分値*/){//@todo:普通にreturnした方がコンパクトにできそう
  double Tc = 1.0/(2.0*PI*fc);
  this->x = (xm-this->x_bfr+(Tc-Ts/2.0)*this->x)/(Ts/2.0+Tc);
  dxm = this->x;
  this->x_bfr = xm;
}

//目標値をramp入力に変換しする
template <class T>
class ramp{
public:
  T x;
  T acc_limit;
  T dec_limit;
  ramp();
  ramp(T x_in, T acc_max, T dec_max);
  void init(T x_in, T acc_max, T dec_max);
  void set_acc(T acc_max, T dec_max);
  void update(T x_in, T& ret_x);

};

template <class T>
ramp<T>::ramp(){
  this->x=0.0;
  this->acc_limit = 0.0;
  this->dec_limit = 0.0;
}

template <class T>
ramp<T>::ramp(T x_in, T acc_limit, T dec_limit){
  this->init( x_in, acc_limit, dec_limit);
}

template <class T>
void ramp<T>::init(T x_in, T acc_limit, T dec_limit){
  x=x_in;
  this->acc_limit = acc_limit;
  this->dec_limit = dec_limit;
}

template <class T>
void ramp<T>::set_acc(T acc_limit, T dec_limit){
  this->acc_limit = acc_limit;
  this->dec_limit = dec_limit;
}

template <class T>
void ramp<T>::update(T x_target, T& ret_x){
  if(x_target > this->x + this->acc_limit * Ts ){
    this->x = this->x + this->acc_limit * Ts;
  }
  else if(x_target < this->x - this->dec_limit * Ts){
    this->x = this->x - this->dec_limit * Ts;
  }
  else{
    this->x = x_target;
  }
  
  ret_x = this->x;
}


}

#endif
