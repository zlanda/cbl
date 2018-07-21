/*******************************************************************************
*   文  件 名：Fifo.c
*   功     能：封装管道功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#include "Fifo.h"

/*******************************************************************************
*   函 数   名：CreateMkFifo
*   功     能：创建一个有名管道
*   输入参数：pcFifoName:管道名
*             iMode:管道文件操作权限
*   输出参数：无
*   返 回 值：-1:创建管道失败
*             >0:创建成功
*   作     者：zhanxc
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
INT32 CreateMkFifo(const CHAR *pcFifoName, mode_t iMode)
{
    /* 参数校验 */
    if (CBL_NULL == pcFifoName)
    {
        return CBL_MKFIFO_FILE_ERR;
    }

	return mkfifo(pcFifoName, iMode);
}

/*******************************************************************************
*   函 数   名：OpenFifo
*   功     能：打开有名管道
*   输入参数：pcFileName:管道名
*             iFlags:
*               O_RDONLY:以只读方式打开文件
*               O_WRONLY:以只写方式打开文件
*               O_RDWR:以可读写方式打开文件
*   输出参数：无
*   返 回 值：-1:打开失败
*             >0:打开成功
*   作     者：zhanxc
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
INT32 OpenFifo(const CHAR *pcFifoName, INT32 iFlags)
{
    /* 参数校验 */
    if (CBL_NULL == pcFifoName)
    {
        return CBL_OPENFIFO_FILE_ERR;
    }

	return open(pcFifoName, iFlags);
}

/*******************************************************************************
*   函 数   名：CloseFifo
*   功     能：关闭有名管道
*   输入参数：iFifoFd:管道文件描述符
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
VOID CloseFifo(INT32 iFifoFd)
{
    /* 参数校验 */
    if (iFifoFd < 0)
    {
        /* 文件描述符已经关闭 */
        return;
    }

    /* 关闭管道文件描述符 */
    close(iFifoFd);

	return;
}

/*******************************************************************************
*   函 数   名：WriteFifo
*   功     能：向管道中写消息
*   输入参数：iFifoFd:管道文件描述符
*             pcBuffer:向管道写入的消息
*             uiBufferSize:消息大小
*   输出参数：无
*   返 回 值：-1:写失败
*             >0:写成功
*   作     者：zhanxc
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
INT32 WriteFifo(INT32 iFifoFd, CHAR *pcBuffer, UINT32 uiBufferSize)
{
    /* 参数校验 */
    if (CBL_NULL == pcBuffer)
    {
        /* 文件描述符已经关闭 */
        return CBL_WRITEFIFO_PARAM_ERR;
    }

	return write(iFifoFd, pcBuffer, uiBufferSize);
}

/*******************************************************************************
*   函 数   名：ReadFifo
*   功     能：从管道中读取消息
*   输入参数：iFifoFd:管道文件描述符
*             pcBuffer:从管道中读取的消息存放的缓存位置
*             uiBufferSize:缓存大小
*   输出参数：无
*   返 回 值：-1:读取失败
*             >0:读取的消息长度
*   作     者：zhanxc
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
INT32 ReadFifo(INT32 iFifoFd, CHAR *pcBuffer, UINT32 uiBufferSize)
{
    /* 参数校验 */
    if (CBL_NULL == pcBuffer || uiBufferSize <= 0)
    {
        /* 文件描述符已经关闭 */
        return CBL_READFIFO_PARAM_ERR;
    }

	return read(iFifoFd, pcBuffer, uiBufferSize);
}

