//nxt motor controller for arduino
//外部割込みによるエンコーダカウント
//@todo:nxtmotor class つくる

#ifndef ___NXT_MOTOR_H
#define ___NXT_MOTOR_H
//#include <Arduino.h>
#include "utilize.h"
//モータ
#define AIN1 13
#define AIN2 12
#define PWMA 10
//ロータリーエンコーダ―
#define TACHOA0_PIN 2
#define TACHOA1_PIN 3

namespace kal{

int enc_cnt=0;
volatile byte pos;
double wheel_angle=0.0;//rad

void ENC_READ();
void motor(double u);

void ENC_READ() {
  byte cur = (!digitalRead(TACHOA1_PIN) << 1) + !digitalRead(TACHOA0_PIN);
  byte old = pos & B00000011;
 
  if (cur == 3) cur = 2;
  else if (cur == 2) cur = 3;
 
  if (cur != old)
  {
    char rote = 1;
    if (cur == 3 && old == 0) rote = 1;
    else if (cur == 0 && old == 3) rote = -1;
    else if (cur > old) rote = -1;
 
    pos = (old << 2) + cur;
 
    const char vector[2] = {'<', '>'};
    //Serial.print(vector[rote]);
    //Serial.println(pos + (1 << 7), BIN);
    //if (cur == 0) Serial.println();
    enc_cnt += rote;  
    wheel_angle = enc_cnt/2.0 * DEG2RAD;  
    //Serial.println(enc_cnt);
  }
}

void motor(double u/*volt*/){
  range(-5.0,5.0,u);
  int duty = 255 * fabs(u) / 5.0;
  if(u > 0.0){//順回転
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2,LOW);
  }
  else if(u < 0.0){//逆回転
     digitalWrite(AIN1,LOW);
     digitalWrite(AIN2,HIGH);
  }
  else{
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,LOW);
  }
  analogWrite(PWMA,duty);
}

}

#endif
