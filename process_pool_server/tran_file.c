#include "pro_pool_server.h"

int tranFile(int new_fd)
{
	int ret;
	train t;
	strcpy(t.buf,DOWNFILE);
	t.dataLen=strlen(DOWNFILE);
	ret=send_n(new_fd,(char *)&t,4+t.dataLen);
	if(-1==ret)
	{
		close(new_fd);
		printf("client close\n");
		return -1;
	}
	int fd=open(DOWNFILE,O_RDONLY);
	struct stat statbuf;
	fstat(fd,&statbuf);
	memcpy(t.buf,&statbuf.st_size,sizeof(statbuf.st_size));
	t.dataLen=sizeof(statbuf.st_size);
	ret=send_n(new_fd,(char *)&t,4+t.dataLen);
	if(-1==ret)
	{
		close(new_fd);
		printf("client close\n");
		return -1;
	}
	while(t.dataLen=read(fd,t.buf,sizeof(t.buf)))
	{
		ret=send_n(new_fd,(char *)&t,4+t.dataLen);
    	if(-1==ret)
    	{
    		close(new_fd);
    		printf("client close\n");
    		return -1;
    	}
	}
	send_n(new_fd,(char *)&t,4+t.dataLen);
	close(new_fd);
	return 0;
}

