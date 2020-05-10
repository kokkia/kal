//kal Ubuntu用UDP通信ライブラリ
//受信時は制御ループの前にudp_bind()
#ifndef ___KAL_UDP_H
#define ___KAL_UDP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include "config.h"

namespace kal{

template<class T>
class udp{
    int sock;
    struct sockaddr_in addr;
public:

    T data;//送るデータの方を事前に決めとく

    udp(std::string address, int port){
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(address.c_str());
        addr.sin_port = htons(port);        
    }

    void string_send(std::string word){//文字列を送る
        sendto(sock, word.c_str(), word.length(), 0, (struct sockaddr *)&addr, sizeof(addr));
    }

    void send(T buf){//T型のオブジェクトを送る
        sendto(sock, &buf, sizeof(buf), 0, (struct sockaddr *)&addr, sizeof(addr));

    }

    void udp_bind(){//受信状態にする
        bind(sock, (const struct sockaddr *)&addr, sizeof(addr));

    }
    std::string string_recv(){//文字列を受け取る:w
        #define BUFFER_MAX 400
        char buf[BUFFER_MAX];
        memset(buf, 0, sizeof(buf));
        recv(sock, buf, sizeof(buf), 0);
        return std::string(buf);
    }
    void string_recv(char *buf, int size){
        memset(buf, 0, size);
        recv(sock, buf, size, 0);
    }

    T receive(){//T型のオブジェクトを受け取る
        memset(&data, 0, sizeof(data));
        recv(sock, &data, sizeof(data), 0);
        return data;
    }

    ~udp(){
        close(sock);
    }
};

}
#endif