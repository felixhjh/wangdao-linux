#include"tranfile.h" 
int tranFile(int new_fd)
{
    train t;
    //发送文件名
    strcpy(t.buf,DOWNFILE);
    t.dataLen=strlen(DOWNFILE);
    send_n(new_fd,(char*)&t,4+t.dataLen);
    //发文件内容
    int fd=open(DOWNFILE,O_RDONLY);
    //发送大小
    struct stat statbuf;
    fstat(fd,&statbuf);
    memcpy(t.buf,&statbuf.st_size,sizeof(statbuf.st_size));
    t.dataLen=sizeof(statbuf.st_size);
    send_n(new_fd,(char*)&t,4+t.dataLen);
    while(t.dataLen=read(fd,t.buf,sizeof(t.buf)))
    {
        send_n(new_fd,(char*)&t,4+t.dataLen);
    }
    send_n(new_fd,(char*)&t,4+t.dataLen);
    close(new_fd);
    return 0;
}

