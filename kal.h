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
#include "joint_controller.h"

//未完成
//#include "kinematics.h"

//option
#ifdef ARDUINO_KAL
#include "nxtmotor.h"

#elif defined ESP32
#include "nxtmotor_for_ESP.h"
#include "color_sensor.h"
#include "two_wheel_drive.h"
#include "servo_for_esp32.h"
#include "udp_for_esp32.h"

#elif defined UBUNTU
#include "udp.h"
#include "gnuplot.h"

#else
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
