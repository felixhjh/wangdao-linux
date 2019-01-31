#include<func.h>
struct msgtype{
	long mtype;
	char buf[1024];
};

int main()
{
	int msgid=msgget((key_t)1000,0666|IPC_CREAT);
	if(msgid==-1)	
	{
		fprintf(stderr,"Create Message Error:%s\n", strerror(errno));
		exit(-1);
	}
	struct msgtype msg;
	printf("I am B\n");
	while(1)
	{
		memset(&msg,0,sizeof(msg));
		msgrcv(msgid,&msg,sizeof(msg.buf),0,0);
		if(msg.mtype==1)
		{
			printf("%s\n", msg.buf);
		}
		if(msg.mtype==2)
		{
			printf("%*s%s\n",30,"",msg.buf);
		}
		if(msg.mtype==3)
		{
		//	printf("%*s%s\n",30,"",msg.buf);
			break;
		}
	}
	msgctl(msgid,IPC_RMID,NULL);
	return 0;
}
