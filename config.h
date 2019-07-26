//robot configulation
#ifndef ___CONFIG_H
#define ___CONFIG_H
//使うコントローラを選択
//#define ARDUINO_KAL
//#define ESP32
#define TITECH
#define EIGEN_KAL
//#define X2_KAL

//Sampling Time
#ifdef ARDUINO_KAL
// #define Ts 0.002//sampling time[ms]
#elif defined ESP32
#define Ts 0.005
#elif defined TITECH
#define Ts 0.001
#endif

#ifndef X2_KAL
#define PI 3.14159265359
#endif


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
