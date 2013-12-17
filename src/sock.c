#include<pthread.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"sock.h"
typedef struct
{
	pthread_cond_t cond;
	int	dump;
	int	port;
	void*	data;
	void(*func)(void*,const char*,size_t,int);
}
args_t;
void*sock_func(args_t*args)
{
	args_t	stack=*args;
	pthread_cond_broadcast(&args->cond);
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	bzero(&servaddr,sizeof servaddr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(stack.port);
	if(!bind(sockfd,(struct sockaddr *)&servaddr,sizeof servaddr))
	while(1)
	{
		if(0<stack.dump)write(stack.dump,"catch:",6);
		char buff[4096];
		unsigned int sum=0;
		len=sizeof cliaddr;
		sum=recvfrom(sockfd,buff,4096,0,(struct sockaddr*)&cliaddr,&len);
		(*stack.func)(stack.data,buff,sum,stack.dump);
		sendto(sockfd,"",1,0,(struct sockaddr*)&cliaddr,&len);
	}
	else if(0<stack.dump)write(stack.dump,"BIND ERROR",10);
	return(void*)0;
}
int sock_gen(int port,void*data,void(*func)(void*,const char*,size_t,int),int dump)
{
	pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
	pthread_t pth;
	args_t	stack={PTHREAD_COND_INITIALIZER,dump,port,data,func};
	pthread_create(&pth,0,sock_func,&stack);
	pthread_cond_wait(&stack.cond,&lock);
	pthread_cond_destroy(&stack.cond);
	pthread_mutex_destroy(&lock);
	return 0;
}
