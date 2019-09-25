//wave generator
#ifndef ___WAVE_H
#define ___WAVW_H
#include "config.h"
#include "utilize.h"

//type
#define CONST 0
#define SIN 1
#define TRIANGLE 2

namespace kal{

class wave{
  public:
    double t;
    int type;//@todo:色々な波形を追加
    double ave;
    double amp;
    double f;
    double output;
    wave();
    wave(double ave_in,double amp_in,double f_in,int type);
    void update();
};

wave::wave(){
  t=0.0;
  ave=0.0;
  amp=0.0;
  f=0.0;
  output=0.0;
  type = SIN;
}

wave::wave(double ave_in,double amp_in,double f_in,int type_in = SIN){
  ave=ave_in;
  amp=fabs(amp_in);
  f=f_in;
  output=0.0;
  type = type_in;
}

void wave::update(){
  t+=Ts;
  //t = t%((int)(1.0/f));//tの∞積分を防止
  switch (type){
  case CONST:
    output = ave;
    break;
  case SIN:
    output = amp*sin(2.0*PI*f*t) + ave;
    break;
  case TRIANGLE://@todo:動作確認済
    if(t>=1.0/f){
      t = 0.0;
    }
    if(0.0<=t && t<=1.0/4.0/f){
      output = amp/(1.0/f/4) * t + ave;  
    }
    else if(1.0/4.0/f<t && t<=3.0/4.0/f){
      output = -amp/(1.0/f/4) * t + (ave + 2*amp);  
    }
    else if(3.0/4.0/f<t && t<=1.0/f){
      output = amp/(1.0/f/4) * t + (ave - 4*amp);
    }
    break;
  default:
    break;
  }
  
}

}

#endif
