//esp32用udp通信用ヘッダ
#ifndef ___KAL_UDP_FOR_ESP32_H
#define ___KAL_UDP_FOR_ESP32_H
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

#define PACKET_SIZE 256
const char ssid[] = "ESP32_wifi"; // SSID
const char pass[] = "esp32pass";  // password
const int localPort = 10000;      // ポート番号
const IPAddress ip(111, 11, 1, 11);       // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク
const IPAddress remoteip(111,11,1,12);
const int remotePort = 10001;

namespace kal {

template<class T>
class udp_for_esp32{
    public:

    //UDP通信関連の定義
    char packetbuffer[PACKET_SIZE]; 
    WiFiUDP udp;
    T data;

    udp_for_esp32();
    void set_udp();
    char receive_char();
    void send(char* sdata);
};

template<class T>
udp_for_esp32<T>::udp_for_esp32(){
}

template<class T>
void udp_for_esp32<T>::set_udp(){
    WiFi.softAP(ssid, pass);           // SSIDとパス設定
    delay(100);                        // delay必要かも
    WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定

    Serial.print("AP IP address: ");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    Serial.println("Starting UDP");
    udp.begin(localPort);  // UDP通信の開始(引数はポート番号)

    Serial.print("Local port: ");
    Serial.println(localPort);
}

template<class T>
char udp_for_esp32<T>::receive_char(){
    int packetsize = udp.parsePacket();
    if (packetsize>0) {
        udp.read(packetbuffer,PACKET_SIZE);
        Serial.print(packetbuffer); // UDP通信で来た値を表示
    }
    return packetbuffer[0];
}

template<class T>
void udp_for_esp32<T>::send(char* sdata){
    udp.beginPacket(remoteip,remotePort);
    udp.write('1');
    udp.endPacket();
}

    
} // namespace name


#endif