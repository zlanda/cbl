#include <stdio.h>
#include <stdlib.h>
#include "Thread.h"

#define MUTEX_TEST                      0
#define RWLOCK_TEST                     1

#if MUTEX_TEST

typedef struct ThreadRun_Sum_Tag
{
    INT32 iSum;             //求和
    pthread_mutex_t Lock;   //互斥锁
} ThreadRun_Sum_S;

VOID *SumAddHandler1(VOID *pvCnt)
{     
    INT32 iIndex = 0;

    //获取互斥锁
    PthreadMutexLock(&(((ThreadRun_Sum_S*)pvCnt)->Lock));

    //求和
    for (iIndex = 0; iIndex < 50; iIndex++)
    {
        (*(ThreadRun_Sum_S*)pvCnt).iSum += iIndex;
        printf("SumAddHandler1:%d\n", (*(ThreadRun_Sum_S*)pvCnt).iSum);
    }

    //释放互斥锁
    PthreadMutexUnLock(&(((ThreadRun_Sum_S*)pvCnt)->Lock));

    //线程退出
    PthreadExit(NULL);
    
    return 0;
}

VOID *SumAddHandler2(VOID *pvCnt)
{     
    INT32 iIndex = 0;

    //获取互斥锁
    PthreadMutexLock(&(((ThreadRun_Sum_S*)pvCnt)->Lock));

    //求和
    for (iIndex = 50; iIndex < 101; iIndex++)
    {
        (*(ThreadRun_Sum_S*)pvCnt).iSum += iIndex;
        printf("SumAddHandler2:%d\n", (*(ThreadRun_Sum_S*)pvCnt).iSum);
    }

    //释放互斥锁
    PthreadMutexUnLock(&(((ThreadRun_Sum_S*)pvCnt)->Lock));

    //线程退出
    PthreadExit(NULL);
    
    return 0;
}

#endif

#if RWLOCK_TEST

pthread_rwlock_t rwlock; //读写锁
INT32 g_iNum = 1;
 
//读操作，其他线程允许读操作，却不允许写操作
VOID *ThreadProcessHandler1(VOID *pvArg)
{
	while(1)
	{
	    //读模式下获取读写锁
		PthreadRwLockRdLock(&rwlock);
		printf("Read num first=%d\n", g_iNum);
		
		//释放读写锁
		PthreadRwLockUnLock(&rwlock);
		sleep(1);
	}
}
 
//读操作，其他线程允许读操作，却不允许写操作
VOID *ThreadProcessHandler2(VOID *pvArg)
{
	while(1)
	{
	    //读模式下获取读写锁
		PthreadRwLockRdLock(&rwlock);
		printf("Read num second=%d\n", g_iNum);

		//释放读写锁
		PthreadRwLockUnLock(&rwlock);
		sleep(2);
	}
}
 
//写操作，其它线程都不允许读或写操作
VOID *ThreadProcessHandler3(VOID *pvArg)
{
	while(1)
	{
		//写模式下获取读写锁
		PthreadRwLockWrLock(&rwlock);
		g_iNum++;
		printf("Write thread first\n");
		
		//释放读写锁
		PthreadRwLockUnLock(&rwlock);
		sleep(2);
	}
}
 
//写操作，其它线程都不允许读或写操作
VOID *ThreadProcessHandler4(VOID *pvArg)
{
	while(1)
	{
		//写模式下获取读写锁
		PthreadRwLockWrLock(&rwlock);
		g_iNum++;
		printf("Write thread second\n");

		//释放读写锁
		PthreadRwLockUnLock(&rwlock);
		sleep(1);
	}
}

#endif

int main()
{
#if MUTEX_TEST
    pthread_t iPtid1;
    pthread_t iPtid2;
    ThreadRun_Sum_S stCnt;
    INT32 iCnt = 0;

    stCnt.iSum = 0;

    //初始化互斥锁
    PthreadMutexInit(&(stCnt.Lock), NULL);

    //创建线程iPtid1
    PthreadCreate(&iPtid1, NULL, SumAddHandler1, &stCnt);
    //创建线程iPtid2
    PthreadCreate(&iPtid2, NULL, SumAddHandler2, &stCnt);

    /* 阻止主进程退出导致创建的线程退出 */
    while (1)
    {
        iCnt++;
        sleep(1);
        if (iCnt > 10)
        {
            break;
        }
    }

    //获取互斥锁
    PthreadMutexLock(&(stCnt.Lock));
    printf("Sum %d\n", stCnt.iSum);
    
    //释放互斥锁
    PthreadMutexUnLock(&(stCnt.Lock));

    //等待线程iPtid1退出
    PthreadJoin(iPtid1, NULL);
    
    //等待线程iPtid2退出
    PthreadJoin(iPtid2, NULL);

    //销毁互斥锁
    PthreadMutexDestroy(&(stCnt.Lock));
#endif

#if RWLOCK_TEST
    pthread_t iPtid1;
    pthread_t iPtid2;
    pthread_t iPtid3;
    pthread_t iPtid4;

    //初始化一个读写锁
    PthreadRwLockInit(&rwlock, NULL);

    //创建线程iPtid1,线程处理函数不带参数
    PthreadCreate(&iPtid1, NULL, ThreadProcessHandler1, NULL);
    //创建线程iPtid2
    PthreadCreate(&iPtid2, NULL, ThreadProcessHandler2, NULL);
    //创建线程iPtid3
    PthreadCreate(&iPtid3, NULL, ThreadProcessHandler3, NULL);
    //创建线程iPtid4
    PthreadCreate(&iPtid4, NULL, ThreadProcessHandler4, NULL);

    //等待线程iPtid1结束，回收其资源
    PthreadJoin(iPtid1, NULL);
    //等待线程iPtid2结束，回收其资源
    PthreadJoin(iPtid2, NULL);
    //等待线程iPtid3结束，回收其资源
    PthreadJoin(iPtid3, NULL);
    //等待线程iPtid4结束，回收其资源
    PthreadJoin(iPtid4, NULL);

    //销毁读写锁
    PthreadRwLockDestroy(&rwlock);

#endif

    return 0;
}

