#ifndef KWS_KAL_KINEMATICS2_H_
#define KWS_KAL_KINEMATICS2_H_
#include<iostream>
#include"linkparameter.h"
#include"transform.h"
#include<math.h>

#ifdef RF
#include<eigen3/Eigen/Dense>
#else
#include"../Eigen/Dense"
#endif

#define ACT_NUM 9

class Joint{
    private:
        Eigen::Vector3d l;
        Eigen::Vector3d loffset;
        Eigen::Vector3d P;
        Eigen::Matrix3d R;
        Eigen::Matrix3d E;

    public:
        void SetJointParam(  Eigen::Vector3d lnew,
                            Eigen::Vector3d loffsetnew,
                            Eigen::Vector3d Pnew,
                            Eigen::Matrix3d Rnew,
                            Eigen::Matrix3d Enew    );
        
        void GetJointParam(  Eigen::Vector3d& lnew,
                            Eigen::Vector3d& loffsetnew,
                            Eigen::Vector3d& Pnew,
                            Eigen::Matrix3d& Rnew,
                            Eigen::Matrix3d& Enew    );

};

void Joint::SetJointParam(  Eigen::Vector3d lnew,
                            Eigen::Vector3d loffsetnew,
                            Eigen::Vector3d Pnew,
                            Eigen::Matrix3d Rnew,
                            Eigen::Matrix3d Enew    )
{
    l=lnew;
    loffset=loffsetnew;
    P=Pnew;
    R=Rnew;
    E=Enew;
}
void Joint::GetJointParam(  Eigen::Vector3d& lnew,
                            Eigen::Vector3d& loffsetnew,
                            Eigen::Vector3d& Pnew,
                            Eigen::Matrix3d& Rnew,
                            Eigen::Matrix3d& Enew    )
{
    lnew=l;
    loffsetnew=loffset;
    Pnew=P;
    Rnew=R;
    Enew=E;
}


class Kinematics{
    private:
        Joint joint[ACT_NUM];

    public:
        void FK_holder();
        void FK_forceps();
        void IK_holder();
        void IK_forceps();
        void jacobi_holder();
        void jacobi();
};

#endif