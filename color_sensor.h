#ifndef ___COLOR_SENSOR_H
#define ___COLOR_SENSOR_H
#include<Wire.h>
#include "config.h"
#include "utilize.h"
#include "filter.h"

#define C_DEBUG 1

namespace kal{

class color_sensor{
    public:
    int r;
    int g;
    int b;
    int a;
    TwoWire Wire2( 1 );//2個目のセンサ

    uint8_t sda_pin;
    uint8_t scl_pin;
    int num = 0;//2個までしか対応してない

    void I2C_setup(uint8_t sda_pin, uint8_t scl_pin);
    void read_color();

};

void color_sensor::I2C_setup(uint8_t sda_pin, uint8_t scl_pin, int num){
    this->sda_pin = sda_pin;
    this->scl_pin = scl_pin;
    this->num = num;
    if(num == 0){
        Wire.begin(sda_pin, scl_pin);
        Wire.beginTransmission(0x2A);
        Wire.write(0x0);
        Wire.write(0x89);
        Wire.endTransmission();
        Wire.beginTransmission(0x2A);
        Wire.write(0x0);
        Wire.write(0x09);
        Wire.endTransmission();
    }
    else if(num == 1){
        Wire.begin(sda_pin, scl_pin);
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