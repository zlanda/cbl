#include "Shmid.h"
#include "Sem.h"

int main(int argc, char *argv[])
{
    key_t iKey;
    INT32 iShmId;
    INT32 iSemId;
    INT32 iValue = 0;
    CHAR *pcShmPtr = CBL_NULL;

    //产生key,根据键值来决定一个信号量或共享内存
    iKey = ftok(".", 0xFF);

    //创建1个信号量,IPC_CREAT标识表明如果信号量不存在，则创建一个信号量，否则获取。
    //0644表明IPC对象的存取权限
    iSemId = CreateSemaphore(iKey, 1, IPC_CREAT|0644);
    if (-1 == iSemId)
    {
        //创建信号量失败时返回-1
        perror("CreateSemaphore");
        exit(EXIT_FAILURE);
    }

    //初始化信号量的值为0
    if (-1 == (InitSemaphoreValue(iSemId, iValue)))
    {
        //设置信号量失败
        perror("ControlSemaphore");
        exit(EXIT_FAILURE);
    }

    //创建共享内存，大小为1024字节
    iShmId = CreateShmid(iKey, 1024, IPC_CREAT|0644);
    if(-1 == iShmId)
    {
        //创建共享内存失败
        perror("CreateShmid");
        exit(EXIT_FAILURE);
    }

    //共享内存与进程建立联系
    pcShmPtr = AtachShmid(iShmId, CBL_NULL, 0);
    if(NULL == pcShmPtr)
    {
        //联系建立失败
        perror("AtachShmid");
        exit(EXIT_FAILURE);
    }
    
    while(1)
    {
        //获取信号量的值是否为1
        if(1 == (iValue = GetSemaphoreValue(iSemId)))
        {
            printf("\nNow, receive message process running:\n");
            //打印共享内存值
            printf("\tThe message is : %s\n", pcShmPtr);

            //操作信号量，将信号量的值减1，stSemBuffer中设置了-1
            if(-1 == SetSemaphoreValue(iSemId, -1))
            {
                perror("OperationSemaphore");
                exit(EXIT_FAILURE);
            }
        }

        //输入end则结束处理 
        if(0 == (strcmp(pcShmPtr ,"end")))
        {
            printf("\nExit the receiver process now!\n");
            break;
        }
    }

    //去掉共享内存与进程的关系
    DetachShmid(pcShmPtr);

    return 0;
}


