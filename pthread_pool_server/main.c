#include "factory.h"

void *threadfunc(void *p)
{
	pFactory pf=(pFactory)p;
	pQue_t pq=&pf->que;
	pNode_t pcur;
	int ret;
	while(1)
	{
		pthread_mutex_lock(&pq->queMutex);
		if(!pq->queSize)
		{
			pthread_cond_wait(&pf->cond,&pq->queMutex);
		}
		ret=queGet(pq,&pcur);
		pthread_mutex_unlock(&pq->queMutex);
		if(0==ret)
		{
			tranFile(pcur->ndSocketfd);
			free(pcur);
		}
		pcur=NULL;
	}
}

int main(int argc, char **argv)
{
	args_check(argc,5);
	Factory f;
	int threadNum=atoi(argv[3]);
	int factoryCapacity=atoi(argv[4]);
	factoryInit(&f,threadNum,factoryCapacity,threadfunc);
	factoryStart(&f);
	int socketfd;
	tcpInit(&socketfd,argv[1],argv[2]);
	int new_fd;
	pQue_t pq=&f.que;
	pNode_t pnew;
	while(1)
	{
		new_fd=accept(socketfd,NULL,NULL);
		pnew=(pNode_t)calloc(1,sizeof(Node_t));
		pnew->ndSocketfd=new_fd;
		pthread_mutex_lock(&pq->queMutex);
		queInsert(pq,pnew);
		pthread_mutex_unlock(&pq->queMutex);
		pthread_cond_signal(&f.cond);

	}
	return 0;

}

