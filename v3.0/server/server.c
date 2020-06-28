#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

//数据帧结构体
struct sock_data_t {
    unsigned char flag;
    int data[3];
};

//定义接收gy39数据的结构体
struct gy39_data_t {
    int illumination;//光照强度
    int temperature;//温度
    int airpressure;//大气压
    int humidity;//湿度
    int elevation;//海拔
};

void send_cmd(void *client_sock_fd_t){
    int client_sock_fd = *((int *)client_sock_fd_t);
    while(1){
        int cmd;
        scanf("%d", &cmd);
        //printf("cmd = %d", cmd);
        int ret = send(client_sock_fd, &cmd, sizeof(cmd), 0);
        if(ret <= 0){
            perror("send error");
            close(client_sock_fd);
            return ;
        }
    }
}

int main()
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
	
	//第二步 绑定套接字 即端口号 
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(12345);//假设我们需要的端口号 是 19658 可以直接复制为19658吗？ 我们这里的 19658是小端模式 所以需要转化为网络字节序 
	server_addr.sin_addr.s_addr =  inet_addr("127.0.0.1");//现在看 我们要做服务端的内容 运行在主机上 就要先查看电脑的IP 
	int ret = bind(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret < 0 )
	{
		perror("bind error");
		return -1;
	}
	else 
	{
		printf("bind success!\n");
	}
	//第三步：监听 
	ret = listen(sock_fd,20);
	if(ret < 0)
	{
		perror("listen error");
		return -1;
	}
	else {
		printf("listen success!\n");
	}
	
	//第四步：建立连接 
	//此时 注意服务器一般是一直都在运行的 客户端只需要进行连接了 连接了服务端就要进行处理 所以 应该一次又一次的处理连接 
	while(1)
	{
		struct sockaddr_in client_addr;
		memset(&client_addr,0,sizeof(client_addr));
		
		int addr_len = sizeof(client_addr);
        //新的连接
		int client_sock_fd = accept(sock_fd,(struct sockaddr *)&client_addr,&addr_len);
		if(client_sock_fd < 0)
		{
			perror("accept error");
			return -1;
		}
		else 
		{
			printf("accept success");
		}
		
        //创建子进程
        pid_t pid = fork();
        if(pid < 0){
            perror("fork error");
            return -1;
            
        }else if(pid == 0){ //子进程
            //子进程任务接受客户端发送数据
            char buf[32];
            while(1){
                memset(buf, 0, 32);
                ret = recv(client_sock_fd, buf, sizeof(struct sock_data_t), 0);
                if(ret <= 0){
                    perror("recv error");
                    close(client_sock_fd);
                    close(sock_fd);
                    return -1;
                }
                struct sock_data_t *data = (struct sock_data_t *)buf;
                if(data->flag == 0){
                    printf("\n烟雾浓度为：%.2f%%\n", data->data[0]/100.0);
                }else if(data->flag == 1){
                    printf("\n温度：%d℃\n湿度：%.2f%%\n光照强度：%dLux\n",data->data[0],data->data[1]/100.0,data->data[2]);
                }else if(data->flag == 2){
                    //创建多线程读取点击坐标
                    pthread_t thread;
                    pthread_create(&thread, NULL, (void *)send_cmd, (void *)&client_sock_fd);  
                    
                    //打印家居状态
                    printf("\n\n");
                    printf("空调现在状态：%s\n", data->data[0]==1 ? "打开" : "关闭");
                    printf("窗帘现在状态：%s\n", data->data[1]==1 ? "打开" : "关闭");
                    printf("台灯现在状态：%s\n", data->data[2]==1 ? "打开" : "关闭");
                    printf("0-----切换空调状态   1-----切换窗帘状态   2-----切换台灯状态\n");
                }else {
                    //其他情况什么也不做
                }

            }
        }else{//父进程
            //什么也不做，继续等待新的连接
        }
	}
	
	close(sock_fd);
}