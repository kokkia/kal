#ifndef _KAL_WIFIUDP2_H_
#define _KAL_WIFIUDP2_H_

// #include <Arduino.h>
#include <Udp.h>
#include <cbuf.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <errno.h>
#include "config.h"

#undef write
#undef read

template<class T>
class kWiFiUDP : public UDP {
private:
  int udp_server;
  IPAddress multicast_ip;
  IPAddress remote_ip;
  uint16_t server_port;
  uint16_t remote_port;
  char * tx_buffer;
  size_t tx_buffer_len;
  cbuf * rx_buffer;
public:
  kWiFiUDP();
  ~kWiFiUDP();
  uint8_t begin(IPAddress a, uint16_t p);
  uint8_t begin(uint16_t p);
  uint8_t beginMulticast(IPAddress a, uint16_t p);
  void stop();
  int beginMulticastPacket();
  int beginPacket();
  int beginPacket(IPAddress ip, uint16_t port);
  int beginPacket(const char *host, uint16_t port);
  int endPacket();
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  int parsePacket();
  int available();
  int read();
  int read(unsigned char* buffer, size_t len);
  int read(char* buffer, size_t len);
  //構造体を送受信
  void send(T buf);
  T receive(); //T型のオブジェクトを受け取る
  int peek();
  void flush();
  IPAddress remoteIP();
  uint16_t remotePort();
};

template<class T>
kWiFiUDP<T>::kWiFiUDP()
: udp_server(-1)
, server_port(0)
, remote_port(0)
, tx_buffer(0)
, tx_buffer_len(0)
, rx_buffer(0)
{}

template<class T>
kWiFiUDP<T>::~kWiFiUDP(){
   stop();
}

template<class T>
uint8_t kWiFiUDP<T>::begin(IPAddress address, uint16_t port){
  stop();

  server_port = port;

  tx_buffer = new char[1460];
  if(!tx_buffer){
    log_e("could not create tx buffer: %d", errno);
    return 0;
  }

  if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    log_e("could not create socket: %d", errno);
    return 0;
  }

  int yes = 1;
  if (setsockopt(udp_server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
      log_e("could not set socket option: %d", errno);
      stop();
      return 0;
  }

  struct sockaddr_in addr;
  memset((char *) &addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(server_port);
  addr.sin_addr.s_addr = (in_addr_t)address;
  if(bind(udp_server , (struct sockaddr*)&addr, sizeof(addr)) == -1){
    log_e("could not bind socket: %d", errno);
    stop();
    return 0;
  }
  fcntl(udp_server, F_SETFL, O_NONBLOCK);
  return 1;
}

template<class T>
uint8_t kWiFiUDP<T>::begin(uint16_t p){
  return begin(IPAddress(INADDR_ANY), p);
}

template<class T>
uint8_t kWiFiUDP<T>::beginMulticast(IPAddress a, uint16_t p){
  if(begin(IPAddress(INADDR_ANY), p)){
    if(a != 0){
      struct ip_mreq mreq;
      mreq.imr_multiaddr.s_addr = (in_addr_t)a;
      mreq.imr_interface.s_addr = INADDR_ANY;
      if (setsockopt(udp_server, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
          log_e("could not join igmp: %d", errno);
          stop();
          return 0;
      }
      multicast_ip = a;
    }
    return 1;
  }
  return 0;
}

template<class T>
void kWiFiUDP<T>::stop(){
  if(tx_buffer){
    delete[] tx_buffer;
    tx_buffer = NULL;
  }
  tx_buffer_len = 0;
  if(rx_buffer){
    cbuf *b = rx_buffer;
    rx_buffer = NULL;
    delete b;
  }
  if(udp_server == -1)
    return;
  if(multicast_ip != 0){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = (in_addr_t)multicast_ip;
    mreq.imr_interface.s_addr = (in_addr_t)0;
    setsockopt(udp_server, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    multicast_ip = IPAddress(INADDR_ANY);
  }
  close(udp_server);
  udp_server = -1;
}

template<class T>
int kWiFiUDP<T>::beginMulticastPacket(){
  if(!server_port || multicast_ip == IPAddress(INADDR_ANY))
    return 0;
  remote_ip = multicast_ip;
  remote_port = server_port;
  return beginPacket();
}

template<class T>
int kWiFiUDP<T>::beginPacket(){
  if(!remote_port)
    return 0;

  // allocate tx_buffer if is necessary
  if(!tx_buffer){
    tx_buffer = new char[1460];
    if(!tx_buffer){
      log_e("could not create tx buffer: %d", errno);
      return 0;
    }
  }

  tx_buffer_len = 0;

  // check whereas socket is already open
  if (udp_server != -1)
    return 1;

  if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    log_e("could not create socket: %d", errno);
    return 0;
  }

  fcntl(udp_server, F_SETFL, O_NONBLOCK);

  return 1;
}

template<class T>
int kWiFiUDP<T>::beginPacket(IPAddress ip, uint16_t port){
  remote_ip = ip;
  remote_port = port;
  return beginPacket();
}

template<class T>
int kWiFiUDP<T>::beginPacket(const char *host, uint16_t port){
  struct hostent *server;
  server = gethostbyname(host);
  if (server == NULL){
    log_e("could not get host from dns: %d", errno);
    return 0;
  }
  return beginPacket(IPAddress((const uint8_t *)(server->h_addr_list[0])), port);
}

template<class T>
int kWiFiUDP<T>::endPacket(){
  struct sockaddr_in recipient;
  recipient.sin_addr.s_addr = (uint32_t)remote_ip;
  recipient.sin_family = AF_INET;
  recipient.sin_port = htons(remote_port);
  int sent = sendto(udp_server, tx_buffer, tx_buffer_len, 0, (struct sockaddr*) &recipient, sizeof(recipient));
  if(sent < 0){
    log_e("could not send data: %d", errno);
    return 0;
  }
  return 1;
}

template<class T>
size_t kWiFiUDP<T>::write(uint8_t data){
  if(tx_buffer_len == 1460){
    endPacket();
    tx_buffer_len = 0;
  }
  tx_buffer[tx_buffer_len++] = data;
  return 1;
}

template<class T>
size_t kWiFiUDP<T>::write(const uint8_t *buffer, size_t size){
  size_t i;
  for(i=0;i<size;i++)
    write(buffer[i]);
  return i;
}

template<class T>
int kWiFiUDP<T>::parsePacket(){
  if(rx_buffer)
    return 0;
  struct sockaddr_in si_other;
  int slen = sizeof(si_other) , len;
  char * buf = new char[1460];
  if(!buf){
    return 0;
  }
  if ((len = recvfrom(udp_server, buf, 1460, MSG_DONTWAIT, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1){
    delete[] buf;
    if(errno == EWOULDBLOCK){
      return 0;
    }
    log_e("could not receive data: %d", errno);
    return 0;
  }
  remote_ip = IPAddress(si_other.sin_addr.s_addr);
  remote_port = ntohs(si_other.sin_port);
  if (len > 0) {
    rx_buffer = new cbuf(len);
    rx_buffer->write(buf, len);
  }
  delete[] buf;
  return len;
}

template<class T>
int kWiFiUDP<T>::available(){
  if(!rx_buffer) return 0;
  return rx_buffer->available();
}

template<class T>
int kWiFiUDP<T>::read(){
  if(!rx_buffer) return -1;
  int out = rx_buffer->read();
  if(!rx_buffer->available()){
    cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

template<class T>
int kWiFiUDP<T>::read(unsigned char* buffer, size_t len){
  return read((char *)buffer, len);
}

template<class T>
int kWiFiUDP<T>::read(char* buffer, size_t len){
  if(!rx_buffer) return 0;
  int out = rx_buffer->read(buffer, len);
  if(!rx_buffer->available()){
    cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

template<class T>
T kWiFiUDP<T>::receive(){//T型のオブジェクトを受け取る
    T data;
    memset(&data, 0, sizeof(data));
    recv(udp_server, &data, sizeof(data), 0);
    return data;
}

template<class T>
void kWiFiUDP<T>::send(T buf){
  struct sockaddr_in recipient;
  recipient.sin_addr.s_addr = (uint32_t)remote_ip;
  recipient.sin_family = AF_INET;
  recipient.sin_port = htons(remote_port);
  // int sent = sendto(udp_server, tx_buffer, tx_buffer_len, 0, (struct sockaddr*) &recipient, sizeof(recipient));
  sendto(udp_server, &buf, sizeof(buf), 0, (struct sockaddr *)&recipient, sizeof(recipient));

}

template<class T>
int kWiFiUDP<T>::peek(){
  if(!rx_buffer) return -1;
  return rx_buffer->peek();
}

template<class T>
void kWiFiUDP<T>::flush(){
  if(!rx_buffer) return;
  cbuf *b = rx_buffer;
  rx_buffer = 0;
  delete b;
}

template<class T>
IPAddress kWiFiUDP<T>::remoteIP(){
  return remote_ip;
}

template<class T>
uint16_t kWiFiUDP<T>::remotePort(){
  return remote_port;
}



#endif /* _WIFIUDP_H_ */