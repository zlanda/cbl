#include <stdio.h>
#include <stdlib.h>
#include "Fifo.h"

#define FIFO_NAME               "fifo"

int main()
{
    /* 新创建的FIFO模式 */
    mode_t iMode = 0666;
    INT32 iRet = 0;
    
    iRet = CreateMkFifo(FIFO_NAME, iMode);
    if (iRet < 0)
    {
        printf("Create Fifo Failed.\r\n");
        return iRet;
    }

    printf("Create Fifo %s Success.\r\n", FIFO_NAME);

    return 0;
}


