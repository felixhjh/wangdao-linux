#include<func.h>

int main()
{
		int shmid=shmget((key_t)1000,sizeof(Text),0600|IPC_CREAT);
		if(shmid==-1)
		{
			perror("shmget");
			exit(-1);
		}
		pText ptext=(pText)shmat(shmid,NULL,0);
		printf("I am A\n");
	while(1){
//		fflush(stdout);
		if(ptext->useful==1)
		{
			if(strlen(ptext->bufA))
			{
			printf("%s\n", ptext->bufA);
			}
			if(strlen(ptext->bufB))
			{
			printf("%*s%s\n",30,"", ptext->bufB);
			}
			ptext->useful=0;
		}
		if(ptext->quitSignal==1)
		{
			break;
		}
	}
	shmdt((void *)ptext);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
