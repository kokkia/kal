//configulation
#ifndef ___CONFIG_H
#define ___CONFIG_H

#define Ts 1//sampling time[ms]

namespace kal{

class RobotData{
public:
  double phi;
  double dphi;
  double d2phi;
  double theta;
  double dtheta;
  double d2theta;
  
  RobotData();  
  
};

RobotData::RobotData(){
  phi = 0.0;
  dphi = 0.0;
  d2phi = 0.0;
  theta = 0.0;
  dtheta = 0.0;
  d2theta = 0.0;
}

}

#endif
