#include "socket.h"

/**
 * socket初始化
 * 创建套接字，连接服务器
 * 参数port: 端口
 * 参数ipaddr：ip地址
 * 返回值：失败返回-1，成功返回socket描述符
*/
int socket_init(int port, char *ipaddr)
{
	//第一步：创建套接字 
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd < 0)
	{
		perror("socket create error");
		return -1;
	}
	else 
	{
		printf("socket creat success!\n");
	}
	
	//第二步 与服务端建立连接 
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);//假设我们需要的端口号 是 19658 可以直接复制为19658吗？ 我们这里的 19658是小端模式 所以需要转化为网络字节序 
	server_addr.sin_addr.s_addr =  inet_addr(ipaddr);//现在看 我们要做服务端的内容 运行在主机上 就要先查看电脑的IP 
									//使用ifconfig 查看  192.168.31.183  我应该怎么赋值？ 
									//直接赋值可以吗？
	int ret = connect(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0 )
	{
		perror("connect error");
		return -1;
	}
	else 
	{
		printf("connect success\n");
	}
	
    return sock_fd;

}

// int socket_send(int sockfd, const char *data)
// {
//     char *send_buf = data;
//     int ret = send(sockfd, send_buf, strlen(send_buf), 0);
//     if (ret < 0){
//         perror("socket_send");
//         return -1;
//     }
//     printf("send success\n");
//     return ret;
// }

// int socket_recv(int sockfd)
// {
//     char *recv_buf = data;
//     int ret = recv(sockfd,recv_buf, strlen(recv_buf), 0);
// }
	// //第三步：收发数据 
	// 	char recv_buf[256]={0};
	// 	char send_buf[256]="Who are you?\n";
	// 	ret = send(sock_fd,send_buf,strlen(send_buf),0);
	// 	if(ret <0)
	// 	{
	// 		perror("send error");
	// 		return -1;
	// 	}
	// 	printf("send success\n");
		
	// 	ret = recv(sock_fd,recv_buf,256,0);
	// 	if(ret < 0)
	// 	{
	// 		perror("recv error");
	// 		return -1;
	// 	}
	// 	printf("recv_buf = %s\n",recv_buf);
		
	// //第四步：关闭套接字
	// 	close(sock_fd);	