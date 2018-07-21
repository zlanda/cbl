#include <stdio.h>
#include <stdlib.h>
#include "Fifo.h"

#define FIFO_NAME               "fifo"

int main()
{
    INT32 iFifoFd = 0;
    CHAR acBuffer[100];
    INT32 iLen = 0;
    
    /* 打开管道 */
    iFifoFd = OpenFifo(FIFO_NAME, O_RDONLY);
    if (iFifoFd < 0)
    {
        printf("Open Fifo Failed.\r\n");
        return 0;
    }

    /* 开始读取,睡眠时间过长可能会引起管道消息丢失 */
    while ((iLen = ReadFifo(iFifoFd, acBuffer, sizeof(acBuffer))) > 0)
    {
        printf("Read:%s\r\n", acBuffer);
        /* 测试管道丢失消息的情况 */
        #if 0
        sleep(3);
        #endif
    }

    /* 关闭Fifo */
    CloseFifo(iFifoFd);
    
    return 0;
}



