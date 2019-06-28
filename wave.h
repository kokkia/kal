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
    int t;
    int type;//@todo:色々な波形を追加
    double ave;
    double amp;
    double f;
    double output;
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
  type = SIN;
}

wave::wave(double ave_in,double amp_in,double f_in){
  ave=ave_in;
  amp=amp_in;
  f=f_in;
  output=0.0;
  type = SIN;
}

void wave::update(){
  t+=Ts;
  t = t%(int)(1.0/f);//tの∞積分を防止
  if(type == SIN){
    output=amp*sin(2.0*PI*f*t)+ave;
  }
  
}

}

#endif
