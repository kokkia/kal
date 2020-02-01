#ifndef KAL_TRANSFORM_H_
#define KAL_TRANSFORM_H_
#include "config.h"
#ifdef X2_KAL
#include<eigen3/Eigen/Dense>
#else
#include"eigen3.3.7/Eigen/Dense"
#endif

namespace kal{
    Eigen::Matrix3d Rx(double q)
    {
        Eigen::Matrix3d rot;
        rot = Eigen::AngleAxisd(q,Eigen::Vector3d(1.0,0.0,0.0));
        return rot;
    }
    Eigen::Matrix3d Ry(double q)
    {
        Eigen::Matrix3d rot;
        rot = Eigen::AngleAxisd(q,Eigen::Vector3d(0.0,1.0,0.0));
        return rot;
    }
    Eigen::Matrix3d Rz(double q)
    {
        Eigen::Matrix3d rot;
        rot = Eigen::AngleAxisd(q,Eigen::Vector3d(0.0,0.0,1.0));
        return rot;
    }
    Eigen::Matrix3d R(Eigen::Vector3d s,double q)
    {
        Eigen::Matrix3d rot;
        rot = Eigen::AngleAxisd(q,s);
        return rot;
    }
}
















#endif