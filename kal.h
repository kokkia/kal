#ifndef ___KAL_H
#define ___KAL_H

#include "config.h"
#include "kalmanfilter.h"
#include "utilize.h"
#include "wave.h"
#include "filter.h"
#include "stepping_motor.h"
#include "trajectory.h"
#include "impedance_control.h"

//未完成
//#include "kinematics.h"
//#include "joint_controller.h"
//#include "two_wheel_drive.h"

//option
#ifdef ARDUINO_KAL
#include "nxtmotor.h"
#elif defined ESP32
#include "nxtmotor_for_ESP.h"
#elif defined TITECH
#endif

#ifdef EIGEN_KAL
#include "eigen3.3.7/Eigen/Core"
#include "eigen3.3.7/Eigen/LU"
#include"transform.h"
#endif

#ifdef X2_KAL
#include"transform.h"
//#include"kinematics.h"
#include"pivot_estimator.h"
#endif

#endif
