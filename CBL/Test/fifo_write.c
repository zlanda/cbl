#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Process.h"
#include "Fifo.h"

#define FIFO_NAME               "fifo"

int main()
{
    INT32 iFifoFd = 0;
    UINT32 uiIndex = 0;
    CHAR acBuffer[100];
    UINT32 uiLen = 0;
    INT32 iWrRet = 0;
    
    /* 打开管道 */
    iFifoFd = OpenFifo(FIFO_NAME, O_WRONLY);
    if (iFifoFd < 0)
    {
        printf("Open Fifo Failed.\r\n");
        return 0;
    }

    for (uiIndex = 0; uiIndex < 10; uiIndex++)
    {
        uiLen = sprintf(acBuffer, "WriteFifo %d uiIndex %d", getpid(), uiIndex);
        printf("Send:%s\r\n", acBuffer);
        iWrRet = WriteFifo(iFifoFd, acBuffer, uiLen + 1);
        if (iWrRet < 0)
        {
            printf("WriteFifo Failed.\r\n");
            exit(1);
        }

        sleep(1);
    }

    CloseFifo(iFifoFd);

    return 0;
}


