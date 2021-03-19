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

#define SOCK_PORT 8538
#define MAX_CONN_LIMIT 10
#define BUFFER_LEN 2048

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
	
	//增加回应系统时间功能
	time_t timep;
	time (&timep);
	char tmp[2048];
	//strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&timep));

	sockfd_server = socket(AF_INET,SOCK_STREAM,0); //使用ipv4，TCP
	assert(sockfd_server != -1);  //判断是否返回成功
	
	
	memset(&s_addr_in,0,sizeof(s_addr_in));  //初始化
	s_addr_in.sin_family = AF_INET; //设置为ipv4
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY); //储存任意ip地址 
	//s_addr_in.sin_port = htons(SOCK_PORT); //设置port
	int port=atoi(argv[1]);
	s_addr_in.sin_port = htons(port); //输入端口绑定

	fd_temp = bind(sockfd_server,(void*)(&s_addr_in),sizeof(s_addr_in));  //绑定地址和端口
	if(fd_temp == -1)
	{
		fprintf(stderr, "[%s:%d] bind error!\n", __FILE__, __LINE__);
		exit(1);
	}

	fd_temp = listen(sockfd_server,MAX_CONN_LIMIT);  //监听此socket
	if(fd_temp == -1)
	{
		fprintf(stderr, "[%s:%d] listen error!\n", __FILE__, __LINE__);
		exit(1);
	}


	printf("waiting a new connect.....\n");
	cilent_length = sizeof(s_addr_cilent);
	sockfd = accept(sockfd_server,(void*) &s_addr_cilent,(socklen_t*) &cilent_length);
	while(1)
	{
		if(sockfd == -1)
		{
		fprintf(stderr, "[%s:%d] accept error!\n", __FILE__, __LINE__);
		continue;
		}
	
		printf("waiting a msg....\n");
		
		memset(recv_data,0,BUFFER_LEN); //初始化数组
		memset(send_data,0,BUFFER_LEN);
		memset(tmp,0,sizeof(tmp));
		i_recv = read(sockfd,recv_data,BUFFER_LEN); //接受cilent信息
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
		strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S :",localtime(&timep));
		//测试是否每次均成功获取时间
		//printf("%s\n",tmp);
		printf("cilent : %s \n",recv_data);
		strcpy(send_data,recv_data);
		strcat(tmp,send_data);
		printf("%s\n",tmp);
		write(sockfd,tmp,sizeof(tmp));
		//write(sockfd,send_data,BUFFER_LEN);
	}
	close(sockfd);
	close(sockfd_server);
}
