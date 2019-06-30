#ifndef ___NXTMOTOR_FOR_ESP_H
#define ___NXTMOTOR_FOR_ESP_H
#include<arduino.h>
#include "utilize.h"

#define PWM_CAREER_FREQ 100000//Hz
#define PWM_RESOLUTION_BIT 8//bit
#define MAX_VOLTAGE 5.0//最大電圧

namespace kal{

class nxtmotor{
public:
    //PWM関連
    uint8_t pin_A;//方向制御pin
    uint8_t pin_B;
    uint8_t pin_PWM;//PWM
    uint8_t channel;
    double PWM_freq;
    uint8_t PWM_resolution_bit;//bit
    uint8_t PWM_resolution;

    nxtmotor();
    void GPIO_setup(uint8_t pin_A,uint8_t pin_B);
    void PWM_setup(uint8_t pin_PWM,uint8_t channel,double PWM_freq = PWM_CAREER_FREQ,uint8_t PWM_resolution_bit = PWM_RESOLUTIONBIT);
    void drive(double u);

    //pulse_counter関連




};

nxtmotor::nxtmotor(){//@todo::ここでPWM_setupできるようにするか検討
}

void nxtmotor::GPIO_setup(uint8_t pin_A,uint8_t pin_B){
    this->pin_A = pin_A;
    this->pin_B = pin_B;
    pinMode(pin_A, OUTPUT);
    pinMode(pin_B, OUTPUT);
}

void nxtmotor::PWM_setup( uint8_t pin_PWM,
                uint8_t channel,
                double PWM_freq = PWM_CAREER_FREQ,
                uint8_t PWM_resolution_bit = PWM_RESOLUTION_BIT){
    this->pin_PWM = pin_PWM;
    this->channel = channel;
    this->PWM_freq = PWM_freq;
    this->PWM_resolution_bit = PWM_resolution_bit;
    //使用するタイマーのチャネルと周波数を設定
    ledcSetup(channel, PWM_freq, PWM_resolution_bit);
    // Pinをチャネルへ接続
    ledcAttachPin(pin, channel);
    //this->PWM_resolution = pow(2,PWM_resolution_bit);
}

void nxtmotor::drive(double u/*volt*/){
   range(-MAX_VOLTAGE,MAX_VOLTAGE,u);
   int duty = 255 * fabs(u) / MAX_VOLTAGE;//PWM_resolution-1
//   if(u > 0.0){//順回転
//     digitalWrite(AIN1,HIGH);
//     digitalWrite(AIN2,LOW);
//   }
//   else if(u < 0.0){//逆回転
//      digitalWrite(AIN1,LOW);
//      digitalWrite(AIN2,HIGH);
//   }
//   else{
//     digitalWrite(AIN1,LOW);
//     digitalWrite(AIN2,LOW);
//   }
//   analogWrite(PWMA,duty);
}

}

#endif