#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include"head.h"
//应用层协议设计,小火车
typedef struct{
    int dataLen;
    char buf[1000];
}train;
#define DOWNFILE "file"
int tranFile(int new_fd);
int send_n(int sfd,char *ptran,int len);
int recv_n(int sfd,char *ptran,int len);
#endif
