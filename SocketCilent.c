#include<stdlib.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<assert.h> 
#include<string.h> 
#include<unistd.h>

//固定端口
#define SOCK_PORT 8538  
#define BUFFER_LEN 2048 

//增加main函数参数用于手动输入端口
int main(int argc,char* argv[])
{
	int sockfd;
	int tempfd;
	struct sockaddr_in s_addr_in;
	char send_data[BUFFER_LEN]; 
	char recv_data[BUFFER_LEN]; 
	
	//初始化发送和接收数据的数组
	memset(send_data,0,BUFFER_LEN);
	memset(recv_data,0,BUFFER_LEN);
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
	fprintf(stderr, "[%s:%d] socket error!\n", __FILE__, __LINE__);
	exit(1);
	}


	memset(&s_addr_in,0,sizeof(s_addr_in));
	
	//设定IP地址
	s_addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
	//设定为IPv4
	s_addr_in.sin_family = AF_INET;
	//设定为固定端口号
	//s_addr_in.sin_port = htons(SOCK_PORT);
	
	//设定为动态端口
	int port = atoi(argv[1]);
	s_addr_in.sin_port = htons(port);
	

	//向服务端发起连接请求
	tempfd = connect(sockfd,(struct sockaddr*) &s_addr_in,sizeof(s_addr_in));
	if(tempfd == -1)
	{
		fprintf(stderr, "[%s:%d] connect error!\n", __FILE__, __LINE__);
		exit(1);
	}
	
	while(1)
	{
		printf("please input anything you wanna tell server.....(input \"quit\" to quit)\n");
		scanf("%[^\n]",send_data);
		getchar();

		//将信息发送给服务端
		tempfd = write(sockfd,send_data,BUFFER_LEN);
		
		if(tempfd == -1)
		{
			fprintf(stderr, "[%s:%d] write error!\n", __FILE__, __LINE__);
			exit(1);
		}

		if(strcmp(send_data,"quit") == 0)
		{
			break;
		}
		else
		{

		//接收来自服务端信息并输出
		tempfd = read(sockfd,recv_data,BUFFER_LEN);
		assert(tempfd != -1);
		printf("%s\n",recv_data);

		//清空接收和发送数据的数组
		memset(send_data,0,BUFFER_LEN);
		memset(recv_data,0,BUFFER_LEN);
		}
	}

	close(sockfd);
}
