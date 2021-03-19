#include<stdlib.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<assert.h> 
#include<string.h> 
#include<unistd.h>

#define SOCK_PORT 8538  //端口预设
#define BUFFER_LEN 2048 

int main()
{
	int sockfd;
	int tempfd;
	struct sockaddr_in s_addr_in;
	char send_data[BUFFER_LEN]; //发送的数据
	char recv_data[BUFFER_LEN]; //接收的数据
	//初始化数组
	memset(send_data,0,BUFFER_LEN);
	memset(recv_data,0,BUFFER_LEN);
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);  //ipv4,tcp
	if(sockfd == -1)
	{
	fprintf(stderr, "[%s:%d] socket error!\n", __FILE__, __LINE__);
	exit(1);
	}

	memset(&s_addr_in,0,sizeof(s_addr_in));
	s_addr_in.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr将一个点间隔地址转换成网络字节序	
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_port = htons(SOCK_PORT);

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
		tempfd = read(sockfd,recv_data,BUFFER_LEN);
		assert(tempfd != -1);
		printf("%s\n",recv_data);
		memset(send_data,0,BUFFER_LEN);
		memset(recv_data,0,BUFFER_LEN);
		}
	}
	close(sockfd);
}
