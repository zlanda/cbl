#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include "Msqid.h"

struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};
 
int main(int argc, char **argv)
{
    INT32 msgid = -1;
    struct msg_st data;
    //注意：类型为0，从测试结果来看可以接收到其他非0类型的消息
    //这里测试的是类型相同的
	LONG msgtype = 100; 
 
    // 建立消息队列
    msgid = CreateMsgQid((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "CreateMsgQid failed width error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
 
    // 从队列中获取消息，直到遇到end消息为止
    while (1)
    {
    	//RecvMsgQid会阻塞
        if (RecvMsgQid(msgid, (void *)&data, BUFSIZ, msgtype, 0) == -1)
        {
            fprintf(stderr, "RecvMsgQid failed width erro: %d", errno);
        }
 
        printf("Receive: %s\n", data.text);
 
        // 遇到end结束
        if (strncmp(data.text, "end", 3) == 0)
        {
            break;
        }
    }
 
    // 删除消息队列
    if (ControlMsgQid(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "ControlMsgQid(IPC_RMID) failed\n");
    }
 
    exit(EXIT_SUCCESS);
}