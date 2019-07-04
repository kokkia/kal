#ifndef ___KAL_H
#define ___KAL_H

#include "config.h"
#include "kalmanfilter.h"
#include "utilize.h"
#include "wave.h"
#include "filter.h"
#include "stepping_motor.h"

//option
#ifdef ARDUINO_KAL
#include "nxtmotor.h"
#elif defined ESP32
#include "nxtmotor_for_ESP.h"
#elif defined TITECH
#endif

#endif
