//convinient function
#ifndef ___UTILIZE_H
#define ___UTILIZE_H

#define DEG2RAD (PI/180.0)
#define RAD2DEG (180.0/PI)

namespace kal{

template <class T>
void range(T mini,T maxi,T& x){
  if(x>maxi){    x=maxi;  }
  if(x<mini){    x=mini;  }
}
//矩形積分
template <class T>
T integral(double t,T v,T x){
  x += v*t;
  return x;
}

//台形積分
template <class T>
T trape_integral(double t,T v_bfr,T v,T x){
  x += (v_bfr + v)*t/2.0;
  return x;
}

//不感帯補償
template <class T>
void dead_zone_compensate(T v_bfr,T v,T& x,T dead_zone){
  if( v_bfr * v < 0.0 ){
    if(v > 5.0*DEG2RAD ){
      x += dead_zone;
    }
    else if(v < -5.0*DEG2RAD){
      x -= dead_zone;
    }
  }
}

}

#endif
