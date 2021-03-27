#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h> 
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<assert.h> 
#include<string.h> 
#include<unistd.h>
#include<time.h>

//固定端口(specified port)
//#define SOCK_PORT 8538
#define MAX_CONN_LIMIT 10
#define BUFFER_LEN 2048

//增加main函数参数用于手动输入端口
int main(int argc,char* argv[])
{

	int sockfd_server;
	int sockfd;
	int fd_temp;
	struct sockaddr_in s_addr_in;
	struct sockaddr_in s_addr_cilent;
	int cilent_length;
	           
	int i_recv;
	char recv_data[BUFFER_LEN];
	char send_data[BUFFER_LEN];
	
	//增加回应系统时间的功能(add function for reply system time)
	time_t timep;
	time (&timep);
	char tmp[2048];
	
	//使用ipv4、TCP
	sockfd_server = socket(AF_INET,SOCK_STREAM,0);
	
	//判断是否返回成功
	assert(sockfd_server != -1); 
       	
	memset(&s_addr_in,0,sizeof(s_addr_in));
	
	//设置为ipv4
	s_addr_in.sin_family = AF_INET;
	//设置IP地址
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY); 
	//使用固定端口
	//s_addr_in.sin_port = htons(SOCK_PORT);
	
	//动态指定端口
	int port=atoi(argv[1]);
	s_addr_in.sin_port = htons(port);

	//绑定地址和端口
	fd_temp = bind(sockfd_server,(void*)(&s_addr_in),sizeof(s_addr_in));
	if(fd_temp == -1)
	{
		fprintf(stderr, "[%s:%d] bind error!\n", __FILE__, __LINE__);
		exit(1);
	}

	//监听此socket，等待客户端的连接请求
	fd_temp = listen(sockfd_server,MAX_CONN_LIMIT);
	if(fd_temp == -1)
	{
		fprintf(stderr, "[%s:%d] listen error!\n", __FILE__, __LINE__);
		exit(1);
	}


	printf("waiting a new connect.....\n");
	cilent_length = sizeof(s_addr_cilent);
	
	//接收请求，并完成连接
	sockfd = accept(sockfd_server,(void*) &s_addr_cilent,(socklen_t*) &cilent_length);
	
	while(1)
	{
		if(sockfd == -1)
		{
			fprintf(stderr, "[%s:%d] accept error!\n", __FILE__, __LINE__);
			continue;
		}
	
		printf("waiting a msg....\n");
		
		//清空接收、发送和时间的数组
		memset(recv_data,0,BUFFER_LEN); 
		memset(send_data,0,BUFFER_LEN);
		memset(tmp,0,sizeof(tmp));

		//接收来自客户端的消息,并对内容进行判断
		i_recv = read(sockfd,recv_data,BUFFER_LEN);
		
		if(i_recv == 0)
		{
			printf("cilent maybe close\n");
			break;
		}
		if(i_recv == -1)
		{
                       fprintf(stderr, "[%s:%d] read error!\n", __FILE__, __LINE__);
	               break;
		}
		if(strcmp(recv_data,"quit") == 0)
		{
			printf("cilent quit!\n");
			break;
		}

		//获取系统时间
		strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S :",localtime(&timep));
		
		//测试是否每次均成功获取时间
		//printf("%s\n",tmp);
		
		printf("cilent : %s \n",recv_data);
		
		//将返回的客户端消息加上时间
		strcpy(send_data,recv_data);
		strcat(tmp,send_data);
		write(sockfd,tmp,sizeof(tmp));
		
		//原样返回客户端消息
		//write(sockfd,send_data,BUFFER_LEN);
	}

	close(sockfd);
	close(sockfd_server);
}
