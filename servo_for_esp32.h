//esp32用サーボモータ制御ライブラリ
//とりあえずSG90用だが，後々汎化
#ifndef ___KAL_SERVO_FOR_ESP_H
#define ___KAL_SERVO_FOR_ESP_H
#include<arduino.h>
#include "utilize.h"
#include "driver/pcnt.h"
#include <math.h>

namespace kal{

class Servo{
public:
    uint8_t pin_PWM;//PWM
    uint8_t channel;//チャンネル
    double PWM_freq;//キャリア周波数
    uint8_t PWM_resolution_bit;//bit
    uint8_t PWM_resolution;

    Servo();
    void PWM_setup(uint8_t pin_PWM,uint8_t channel,double PWM_freq,uint8_t PWM_resolution_bit);
    void position_control(double rad);
    void position_control_deg(double deg);
};

Servo::Servo(){
}

void Servo::PWM_setup(   uint8_t pin_PWM,
                            uint8_t channel,
                            double PWM_freq,
                            uint8_t PWM_resolution_bit){
    this->pin_PWM = pin_PWM;
    this->channel = channel;
    this->PWM_freq = PWM_freq;
    this->PWM_resolution_bit = PWM_resolution_bit;
    //使用するタイマーのチャネルと周波数を設定
    ledcSetup(channel, PWM_freq, PWM_resolution_bit);
    // Pinをチャネルへ接続
    ledcAttachPin(pin_PWM, channel);
    this->PWM_resolution = pow(2,PWM_resolution_bit);
}

void Servo::position_control(double rad){
    int duty = (double)255 * (0.5 + 1.9 * (rad + 90*DEG2RAD)/PI)/20.0; 
    ledcWrite(channel, duty);
}

void Servo::position_control_deg(double deg){
    int duty = (double)255 * (0.5 + 1.9 * (deg + 90.0)/180.0)/20.0; 
    ledcWrite(channel, duty);
}


}

#endif