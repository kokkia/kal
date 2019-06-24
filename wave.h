//wave generator
#ifndef ___WAVE_H
#define ___WAVW_H

#include "config.h"
#include "utilize.h"

namespace kal{

class wave{
  public:
    int t;
    double ave;
    double amp;
    double f;
    double output;
    char type;//いずれ色々な波形追加
    wave();
    wave(double ave_in,double amp_in,double f_in);
    void update();
};

wave::wave(){
  t=0;
  ave=0.0;
  amp=0.0;
  f=0.0;
  output=0.0;
}

wave::wave(double ave_in,double amp_in,double f_in){
  ave=ave_in;
  amp=amp_in;
  f=f_in;
  output=0.0;
}

void wave::update(){
  t+=Ts;
  t = t%(int)(1.0/f*1000);//tの∞積分を防止
  output=amp*sin(2*PI*f*t/1000.0)+ave;
}

}

#endif
