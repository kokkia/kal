//nxtmotor controller for esp32
#ifndef ___NXTMOTOR_FOR_ESP_H
#define ___NXTMOTOR_FOR_ESP_H
#include<arduino.h>
#include "utilize.h"
#include "driver/pcnt.h"

#define PWM_CAREER_FREQ 50000//Hz
#define PWM_RESOLUTION_BIT 8//bit
#define MAX_VOLTAGE 5.0//最大電圧

namespace kal{

class nxtmotor{
public:
    //PWM関連
    uint8_t pin_fwd;//方向制御pin
    uint8_t pin_bwd;
    uint8_t pin_PWM;//PWM
    uint8_t channel;//チャンネル
    double PWM_freq;//キャリア周波数
    uint8_t PWM_resolution_bit;//bit
    uint8_t PWM_resolution;

    nxtmotor();
    void GPIO_setup(uint8_t pin_fwd,uint8_t pin_bwd);
    void PWM_setup(uint8_t pin_PWM,uint8_t channel,double PWM_freq,uint8_t PWM_resolution_bit);
    void drive(double u);

    //pulse_counter関連
    double angle = 0.0;//回転角度[rad]
    double angle_deg = 0.0;//回転角度[deg]
    void encoder_setup(pcnt_unit_t pcnt_unit,uint8_t pin_A_phase,uint8_t pin_B_phase);
    void get_angle(pcnt_unit_t pcnt_unit,double& angle);
};

nxtmotor::nxtmotor(){//@todo::ここでPWM_setupできるようにするか検討
}

//方向制御ピン設定
void nxtmotor::GPIO_setup(uint8_t pin_fwd,uint8_t pin_bwd){
    this->pin_fwd = pin_fwd;
    this->pin_bwd = pin_bwd;
    pinMode(pin_fwd, OUTPUT);
    pinMode(pin_bwd, OUTPUT);
}

//PWMの設定
void nxtmotor::PWM_setup(   uint8_t pin_PWM,
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
    ledcAttachPin(pin_PWM, channel);
    //this->PWM_resolution = pow(2,PWM_resolution_bit);
}

//駆動
void nxtmotor::drive(double u/*volt*/){
   range(-MAX_VOLTAGE,MAX_VOLTAGE,u);
   int duty = 255 * fabs(u) / MAX_VOLTAGE;//PWM_resolution-1
  if(u > 0.0){//順回転
    digitalWrite(pin_fwd,HIGH);
    digitalWrite(pin_bwd,LOW);
  }
  else if(u < 0.0){//逆回転
     digitalWrite(pin_fwd,LOW);
     digitalWrite(pin_bwd,HIGH);
  }
  else{
    digitalWrite(pin_fwd,LOW);
    digitalWrite(pin_bwd,LOW);
  }
  //analogWrite(PWMA,duty);
  ledcWrite(channel, duty);
}

//2相インクリメンタルエンコーダ
//4逓倍パルスカウンタの設定
void nxtmotor::encoder_setup(pcnt_unit_t pcnt_unit,uint8_t pin_A_phase/*A相*/,uint8_t pin_B_phase/*B相*/){
    pcnt_config_t pcnt_confA;
    pcnt_config_t pcnt_confB;

    pcnt_confA.unit = pcnt_unit;
    pcnt_confA.channel = PCNT_CHANNEL_0;
    pcnt_confA.pulse_gpio_num = pin_A_phase;
    pcnt_confA.ctrl_gpio_num = pin_B_phase;
    pcnt_confA.pos_mode = PCNT_COUNT_INC;
    pcnt_confA.neg_mode = PCNT_COUNT_DEC;
    pcnt_confA.lctrl_mode = PCNT_MODE_REVERSE;
    pcnt_confA.hctrl_mode = PCNT_MODE_KEEP;
    pcnt_confA.counter_h_lim = 32767;
    pcnt_confA.counter_l_lim = -32768;

    pcnt_confB.unit = pcnt_unit;
    pcnt_confB.channel = PCNT_CHANNEL_1;
    pcnt_confB.pulse_gpio_num = pin_B_phase;
    pcnt_confB.ctrl_gpio_num = pin_A_phase;
    pcnt_confB.pos_mode = PCNT_COUNT_INC;
    pcnt_confB.neg_mode = PCNT_COUNT_DEC;
    pcnt_confB.lctrl_mode = PCNT_MODE_KEEP;
    pcnt_confB.hctrl_mode = PCNT_MODE_REVERSE;
    pcnt_confB.counter_h_lim = 32767;
    pcnt_confB.counter_l_lim = -32768;

    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_confA);
    pcnt_unit_config(&pcnt_confB);

    pcnt_counter_pause(pcnt_unit);
    pcnt_counter_clear(pcnt_unit);
    pcnt_counter_resume(pcnt_unit);
}

void nxtmotor::get_angle(pcnt_unit_t pcnt_unit,double& ret_angle){
    int16_t count;
    pcnt_get_counter_value(pcnt_unit, &count);
    angle = (double)count / 2.0 * DEG2RAD;
    angle_deg = (double)count / 2.0;
    ret_angle = angle;
}

}

#endif