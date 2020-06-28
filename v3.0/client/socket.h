#ifndef __SOCKET_H
#define __SOCKET_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT    12345
//#define IPADDR  "47.98.250.215"
#define IPADDR  "127.0.0.1"

//数据帧结构体
struct sock_data_t {
    unsigned char flag;
    int data[3];
};

int socket_init(int port, char *ipaddr);


#endif // !__SOCKET_H



