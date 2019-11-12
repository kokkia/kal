//esp32
//カラーセンサS11059-02DT用ライブラリ
//使い方注意
//現状最大2個
#ifndef ___COLOR_SENSOR_H
#define ___COLOR_SENSOR_H
#include<Wire.h>
#include "config.h"

#define C_DEBUG 1
TwoWire Wire2( 1 );//2個目のセンサ

namespace kal{

class color_sensor{
    public:
    int r;//赤
    int g;//緑
    int b;//青
    int a;//赤外線

    uint8_t sda_pin;
    uint8_t scl_pin;
    int num = 0;//2個までしか対応してない
    static int counter;//静的メンバ，カラーセンサーの番号を付ける

    void I2C_setup(uint8_t sda_pin, uint8_t scl_pin);//pinの設定
    void read_color();//メンバ変数で取得

};

int color_sensor::counter = 0;//静的メンバ，カラーセンサーの番号を付ける

//これはメソッドではない
//setup後に必ずinitする
void color_sensor_init(color_sensor cl){
    if(cl.num == 0){
        Wire.begin(cl.sda_pin, cl.scl_pin);
        Wire.beginTransmission(0x2A);
        Wire.write(0x0);
        Wire.write(0x89);
        Wire.endTransmission();
        Wire.beginTransmission(0x2A);
        Wire.write(0x0);
        Wire.write(0x09);
        Wire.endTransmission();
    }
    else if(cl.num == 1){
        Wire2.begin(cl.sda_pin, cl.scl_pin);
        Wire2.beginTransmission(0x2A);
        Wire2.write(0x0);
        Wire2.write(0x89);
        Wire2.endTransmission();
        Wire2.beginTransmission(0x2A);
        Wire2.write(0x0);
        Wire2.write(0x09);
        Wire2.endTransmission();
    }

}

void color_sensor::I2C_setup(uint8_t sda_pin, uint8_t scl_pin){
    this->sda_pin = sda_pin;
    this->scl_pin = scl_pin;
    this->num = counter++;
    //この後必ずinit
    color_sensor_init(*this);
}

void color_sensor::read_color(){
    if(num == 0){
        int h,l;
        Wire.beginTransmission(0x2A);
        Wire.write(0x03);
        Wire.endTransmission();
        Wire.requestFrom(0x2A,8);
        if(Wire.available()){
            //赤
            h = Wire.read();
            l = Wire.read();
            r = h << 8|l;
            //緑
            h = Wire.read();
            l = Wire.read();
            g = h << 8|l;
            //青
            h = Wire.read();
            b = Wire.read();
            r = h << 8|l;
            //赤外
            h = Wire.read();
            l = Wire.read();
            a = h << 8|l;
        }
        Wire.endTransmission();
    }
    else if(num == 1){
        int h,l;
        Wire2.beginTransmission(0x2A);
        Wire2.write(0x03);
        Wire2.endTransmission();
        Wire2.requestFrom(0x2A,8);
        if(Wire2.available()){
            //赤
            h = Wire2.read();
            l = Wire2.read();
            r = h << 8|l;
            //緑
            h = Wire2.read();
            l = Wire2.read();
            g = h << 8|l;
            //青
            h = Wire2.read();
            b = Wire2.read();
            r = h << 8|l;
            //赤外
            h = Wire2.read();
            l = Wire2.read();
            a = h << 8|l;
        }
        Wire2.endTransmission();
    }

#if C_DEBUG
    Serial.print("num:");
    Serial.print(num);
    Serial.print("r:");
    Serial.print(r);
    Serial.print(" g:");
    Serial.print(g);
    Serial.print(" b:");
    Serial.print(b);
    Serial.print(" a:");
    Serial.print(a);
    Serial.println("");
#endif
            
}

}


#endif