#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<signal.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
typedef struct{
	pid_t pid;
	int fds;//子进程的管道对端
	short busy;//0代表子进程不忙碌，1代表子进程忙碌
}proData;

void makeChild(proData *,int );
void childHandle(int);
int tcpInit(int *,char *,char *);
void send_fd(int,int);
void recv_fd(int,int*);




