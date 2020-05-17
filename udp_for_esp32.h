//esp32用udp通信用ヘッダ
#ifndef ___KAL_UDP_FOR_ESP32_H
#define ___KAL_UDP_FOR_ESP32_H
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <WiFi.h>
// #include <WiFiUdp.h>
#include "WiFiUdp2.h"
#include "config.h"

#define NO_NETWORK 0
#define ISOLATED_NETWORK 1
#define CONNECT_NETWORK 2

#define PACKET_SIZE 256

//適当によく使う設定
const char esp_ssid[] = "ESP32_wifi"; // SSID
const char esp_pass[] = "esp32pass";  // password
const IPAddress ip(111, 11, 1, 11);       // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク
const IPAddress remoteip(111,11,1,12);
const int localPort = 4000;      // ポート番号
const int remotePort = 4001;

namespace kal {

template<class T>
class udp_for_esp32{
    public:
    int network;//独立ネットワークにするか無線LANにつなぐか
    
    char packetbuffer[PACKET_SIZE];
    IPAddress rip = remoteip;
    kWiFiUDP<T> udp;
    // T* sbuffer;//送信用
    // T rbuffer;//受信用

    udp_for_esp32(int network);
    void set_udp(const char* ssid,const char* pass);
    void set_remoteip(IPAddress rip);
    char receive_char();
    T receive();
    void send_char(char c);
    void send(T sbuf);
};

template<class T>
udp_for_esp32<T>::udp_for_esp32(int network){
    this->network = network; 
}

template<class T>
void udp_for_esp32<T>::set_udp(const char* ssid,const char* pass){
    if(network == ISOLATED_NETWORK){//独立したネットワークを作成
        WiFi.softAP(ssid, pass);           // SSIDとパス設定
        delay(100);                        // delay必要かも
        WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定
        Serial.print("AP IP address: ");
        IPAddress myIP = WiFi.softAPIP();
        Serial.println(myIP);
    }
    else if(network == CONNECT_NETWORK){//  無線 LAN に接続
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);             //  接続確立まで待つ
        Serial.println("Connecting...");
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
        }
        Serial.println("Connected");
        Serial.println(WiFi.localIP());     //  ESP 自身の IP アドレスをログ出力
        // //  mDNS の呼びかけに応答できるように仕込む
        // Serial.println("Setting up mDNS...");
        // if (MDNS.begin("esp32")){         //  "dongbei1.local" で応答
        //     Serial.println("mDNS started");
        // }else{
        //     Serial.println("mDNS failed to start");
        // }
    }
    Serial.println("Starting UDP");
    udp.begin(localPort);  // UDP通信の開始(引数はポート番号)
    Serial.print("Local port: ");
    Serial.println(localPort);
}

template<class T>
void udp_for_esp32<T>::set_remoteip(IPAddress rip){
    this->rip = rip;
    Serial.println(this->rip);
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
T udp_for_esp32<T>::receive(){
    T buffer;
    buffer = udp.receive();
    return buffer;

}

template<class T>
void udp_for_esp32<T>::send_char(char c){
    udp.beginPacket(rip,remotePort);
    udp.write(c);
    udp.endPacket();
}

template<class T>
void udp_for_esp32<T>::send(T sbuf){
    udp.beginPacket(rip,remotePort);
    udp.send(sbuf);
}
    
} // namespace name


#endif