#include <func.h>
pid_t Apid;
struct msgtype{
	long mtype;
	char buf[1024];
};

void sigfunc(int signum,siginfo_t *pInfo,void *pReserved)
{
	kill(Apid,SIGINT);
	int msgid=msgget((key_t)1000,0666|IPC_CREAT);
	struct msgtype msg;
	memset(&msg,0,sizeof(struct msgtype));
	msg.mtype=3;
	msgsnd(msgid,&msg,0,0);
	int semid=semget(1000,1,IPC_CREAT|0600);
	semctl(semid,0,IPC_RMID);
//	kill(getpid(),SIGQUIT);
	exit(0);
}
int main(int argc,char** argv)
{
	int msgid=msgget((key_t)1000,0666|IPC_CREAT);
	if(msgid==-1)
	{
		fprintf(stderr,"Creat Massage  Error:%s\n",strerror(errno));
		exit(-1);
	}
	struct msgtype msg;
	memset(&msg,0,sizeof(struct msgtype));
//	msg.mtype=1;
	args_check(argc,3);
	int fdw=open(argv[1],O_WRONLY);//以写的方式打开一号管道
	if(-1==fdw)
	{
		perror("open");
		return -1;
	}
	int fdr=open(argv[2],O_RDONLY);
	if(-1==fdr)
	{
		perror("open1");
		return -1;
	}
	char buf[128]={0};
	int ret;
	fd_set rdset;//描述符监控的读集合
	struct timeval t;
	int semid=semget(1000,1,IPC_CREAT|0600);
	if(-1==semid)
	{
		perror("semget");
		return -1;
	}
	 ret=semctl(semid,0,SETVAL,1);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
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
	pid_t Bpid=getpid();
	write(fdw,&Bpid,sizeof(Bpid));
	read(fdr,&Apid,sizeof(Apid));
//	printf("Apid=%d,Bpid=%d\n", Apid, Bpid);
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		memset(&t,0,sizeof(t));
		memset(&msg,0,sizeof(msg));
		t.tv_sec=3;
		ret=select(fdr+1,&rdset,NULL,NULL,&t);
		if(ret>0)
		{
			if(FD_ISSET(STDIN_FILENO,&rdset))
			{
				memset(buf,0,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));//读取标准输入
				write(fdw,buf,strlen(buf)-1);//写端管道内
				semop(semid,&sopp,1);
				msg.mtype=1;
				strncpy(msg.buf,buf,sizeof(buf));
				msgsnd(msgid,&msg,strlen(msg.buf)-1,0);
				semop(semid,&sopv,1);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				memset(buf,0,sizeof(buf));
				ret=read(fdr,buf,sizeof(buf));
				semop(semid,&sopp,1);
				msg.mtype=2;
				strncpy(msg.buf,buf,sizeof(buf));
				msgsnd(msgid,&msg,strlen(msg.buf),0);
				semop(semid,&sopv,1);
			}
		}else{
//			printf("select timeout\n");
		}

	}
	close(fdr);
	close(fdw);
	return 0;
}
