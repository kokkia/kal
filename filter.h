//filter and differentiator
#ifndef ___FILTER_H
#define ___FILTER_H
#include "config.h"
#include "utilize.h"

namespace kal{

//filter base
template <class T>
class filter_base{
public:
  T x;
  T x_bfr;
  void update();
  filter_base();
  filter_base(T x_in);    
};

template <class T>
filter_base<T>::filter_base(){
  x=0.0;
  x_bfr=0.0;
}
template <class T>
filter_base<T>::filter_base(T x_in){
  x=x_in;
  x_bfr=x_in;
}

//LPF
template <class T>
class LPF : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  LPF();
  LPF(T x_in, double fc_in);
  void update(T xm);
};

template<class T>
LPF<T>::LPF(T x_in, double fc_in) : filter_base<T>(x_in){
  fc=fc_in;
}


template <class T>
void LPF<T>::update(T xm){
  double Tc = 1.0/(2*PI*fc);
  this->x=Ts/(Ts+2*Tc)*xm +2*Tc/(Ts+2*Tc)*this->x;
}


//HPF
template <class T>
class HPF : public filter_base<T>{
public:
  double fc = 1.0;//カットオフ周波数
  HPF();
  HPF(T x_in, double fc_in);
  void update(T xm);
};

template<class T>
HPF<T>::HPF(T x_in, double fc_in) : filter_base<T>(x_in){
  fc=fc_in;
}

template <class T>
void HPF<T>::update(T xm){
  double Tc = 1.0/(2*PI*fc);
  this->x=2.0*Tc/(Ts+2.0*Tc)*xm - Ts/(Ts+2.0*Tc)*this->x;
}



//differentiator
template <class T>
class Diff : public filter_base<T>{
public:
  double fc=1.0;//カットオフ周波数
  Diff();
  Diff(T x_in, double fc_in);
  void update(T xm);
};

template<class T>
Diff<T>::Diff(T x_in, double fc_in) : filter_base<T>(x_in){
  fc=fc_in;
}

template <class T>
void Diff<T>::update(T xm){
  double Tc = 1.0/(2.0*PI*fc);
  this->x=(xm-this->x_bfr+(Tc-Ts/2.0)*this->x)/(Ts/2.0+Tc);
  //this->x=(xm-this->x_bfr)/((double)Ts);
  this->x_bfr=xm;
}

}

#endif
