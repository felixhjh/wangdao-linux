#include<func.h>
pid_t Bpid;
void sigfunc(int signum,siginfo_t *pInfo,void *pReserved)
{
	kill(Bpid,SIGINT);
	int shmid=shmget((key_t)1000,sizeof(Text),0600|IPC_CREAT);
	pText ptext=(pText)shmat(shmid,NULL,0);
	ptext->quitSignal=1;
//	kill(getpid(),SIGQUIT);
	exit(0);
}

int main(int argc, char** argv)
{
	int shmid=shmget((key_t)1000,sizeof(Text),0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	pText ptext=(pText)shmat(shmid,NULL,0);
	if((pText)-1==ptext)
	{
		perror("shmat");
		return -1;
	}
	memset(ptext,0,sizeof(Text));
//	ptext->useful=0;
	args_check(argc,3);
	int fdr=open(argv[1],O_RDONLY);
	if(-1==fdr)
	{
		perror("open");
		return -1;
	}
	int fdw=open(argv[2],O_WRONLY);
	if(-1==fdw)
	{
		perror("open1");
		return -1;
	}
	char buf[1024]={0};
	int ret;
	fd_set rdset;
	struct timeval t;
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_sigaction=sigfunc;
	act.sa_flags=SA_SIGINFO;
	ret=sigaction(SIGINT,&act,NULL);
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	pid_t Apid=getpid();
	write(fdw,&Apid,sizeof(Apid));
	read(fdr,&Bpid,4);

	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		memset(&t,0,sizeof(t));
		t.tv_sec=3;
		ret=select(fdr+1,&rdset,NULL,NULL,&t);
		memset(ptext->bufA,0,1024);
		memset(buf,0,sizeof(buf));
		memset(ptext->bufB,0,1024);
		if(ret>0)
		{
			if(FD_ISSET(STDIN_FILENO,&rdset))
			{

				ret=read(STDIN_FILENO,buf,sizeof(buf));
		//		if(0==ret)
		//		{
		//			printf("this is end\n");
		//			ptext->quitSignal=1;
		//		}
				write(fdw,buf,strlen(buf)-1);
				if(ptext->useful==0)
				{
					memcpy(ptext->bufA,buf,strlen(buf)-1);
					ptext->useful=1;
				}
			}
			if(FD_ISSET(fdr,&rdset))
			{
				ret=read(fdr,buf,sizeof(buf));
				if(ptext->useful==0)
				{
					memcpy(ptext->bufB,buf,strlen(buf));
					ptext->useful=1;
				}
			}
		}else{
//				printf("select timeout\n");
			}
		}
		close (fdr);
		close(fdw);
		return 0;
	}
