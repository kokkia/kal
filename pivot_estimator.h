#ifndef KWS_KAL_FILTER_H_
#define KWS_KAL_FILTER_H_
#include<iostream>
#include "config.h"

#ifdef X2_KAL
#include<eigen3/Eigen/Dense>
#else
#include"../Eigen/Dense"
#endif
//#define Ts 0.001

bool inserted_flag = 0;//挿入されているかいないか
bool fr_flag = 0;//手先に負荷があるかないか

// void lpf(double T,double x/*過去*/, double& y/*実測->lpf*/){
//     //Tsはマクロ
//     double A=Ts/(Ts+2*T);
//     double B=2*T/(Ts+2*T);
//     y=A*x+B*y;
// }

// int sgn(double x){
//   if( fabs(x) < 1e-9 )
//     return 0;
//   else if( x > 0 ) 
//     return 1;
//   else
//     return -1;
// }

double threshold_filter(double x,double threshold,int n){
  if(threshold == 0.0){
      return x;
  }
  const double r = fabs(x) / threshold;
  return ( r < 1.0 ) ?  pow( r , n ) * x : x;

}

void pibot_estimation(Eigen::Vector3d Pw,Eigen::Matrix3d Rw,Eigen::Vector3d& Pp,double& dd){
    static double d=331.3;
    //double k=1.0;
    static double ip=0.0;
    static Eigen::Vector3d Pw_b=Pw;
    static Eigen::Vector3d Pp_b=Pp;
    static int cnt2=0;
    
    Eigen::Vector3d n=Rw.col(0);//鉗子の方向ベクトル
    Pp=Pw+(-1)*d*n;

    Eigen::Vector3d dPp=Pp-Pp_b;
    Eigen::Vector3d dPw=Pw-Pw_b;
    // std::cout<<"dPp="<<dPp<<std::endl;
    // std::cout<<"dPw="<<dPw<<std::endl;
   
    Eigen::Vector3d dPwt=dPw-dPw.dot(n)*n;
    Eigen::Vector3d dPpt=dPp-dPp.dot(n)*n;

    //double dd=k*dPwt.dot(dPpt);
    ip = 0.995 * ip + 0.005 * ( dPwt.dot(dPpt));//LPF
    if(ip<-0.005){ip=-0.005;}
    if(ip>0.005){ip=0.005;}
    d+=10*ip+dPw.dot(n);//左右+x方向並進
    //d+=dPw.dot(n);//並進のみ更新
    if(d<150.0){d=150.0;}
    if(d>430){d=430;}

    Pp = Pw + (-1)*d*n;
    dd = d;

    cnt2++;
    Pw_b = Pw;
    Pp_b = Pp;

}


#endif