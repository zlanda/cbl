#include "Shmid.h"
#include "Sem.h"

int main(int argc, char *argv[])
{
    key_t iKey;
    INT32 iShmId;
    INT32 iSemId;
    INT32 iValue = 0;
    CHAR *pcShmPtr = CBL_NULL;

    //产生key
    iKey = ftok(".", 0xFF);

    //创建信号量
    iSemId = CreateSemaphore(iKey, 1, IPC_CREAT|0644);
    if(-1 == iSemId)
    {
        perror("CreateSemaphore");
        exit(EXIT_FAILURE);
    }

    //设置信号量的值为0
    if(-1 == (InitSemaphoreValue(iSemId, iValue)))
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    //创建1024字节共享内存
    iShmId = CreateShmid(iKey, 1024, IPC_CREAT|0644);
    if(-1 == iShmId)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //建立共享内存与进程之间的关系
    pcShmPtr = AtachShmid(iShmId, CBL_NULL, 0);
    if (CBL_NULL == pcShmPtr)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        //获取信号量的值是否为0
        if(0 == (iValue = GetSemaphoreValue(iSemId)))
        {
            printf("\nNow, snd message process running:\n");
            printf("\tInput the snd message:  ");
            //想共享内存中输入值
            scanf("%s", pcShmPtr);

            //操作信号量，将信号量的值增1
            if(-1 == SetSemaphoreValue(iSemId, 1))
            {
                perror("OperationSemaphore");
                exit(EXIT_FAILURE);
            }
        }

        //如果输入了"end",则退出结束进程
        if(0 == (strcmp(pcShmPtr ,"end")))
        {
            printf("\nExit sender process now!\n");
            break;
        }
    }

    //删除共享内存与进程之间的关系
    DetachShmid(pcShmPtr);

    //删除共享内存
    if(-1 == ControlShmid(iShmId, IPC_RMID, CBL_NULL))
    {
        perror("ControlShmid");
        exit(EXIT_FAILURE);
    }

    //删除信号量
    if(-1 == DelSemaphore(iSemId))
    {
        perror("ControlSemaphore IPC_RMID");
        exit(EXIT_FAILURE);
    }

    return 0;
}


