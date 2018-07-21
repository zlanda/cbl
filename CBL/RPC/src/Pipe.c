/*******************************************************************************
*   文  件 名：Pipe.c
*   功     能：封装匿名管道功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/

#include "Pipe.h"

/*******************************************************************************
*   函 数   名：CreatePipe
*   功     能：创建匿名管道
*   输入参数：iPipe:文件描述符，iPipe[0]表示读出端，iPipe[1]表示写入端
*   输出参数：无
*   返 回 值：0：成功，当函数成功返回时，自动维护了一个从iPipe[1]到iPipe[0]的数据通道
*             -1：失败
*			  注意：文件描述符iPipe并不需要和任何有名文件相关联，之后向管道一端写入
*					数据并从读出端读出数据，将数据输出到标准输出。最后使用close()函数
*					关闭管道的两端
*					单独一个进程操作管道是没有任何意义的，管道的应用一般体现在父子
*					进程或者兄弟进程的通信，如果要建立一个父进程到子进程的数据通道，
*					可以先调用pipe()函数，紧接着调用fork()函数，由于子进程自动继承
*					父进程的数据段，则父子进程同时拥有管道的操作权
*					在进行读写管道时，对一个管道进行读操作后，read()函数返回0，有
*					两种意义，一种是管道中无数据并且写入端已经关闭；另一种是管道中
*					无数据，写入端依然存活。
*   作     者：zhanxc
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/
INT32 CreatePipe(INT32 iPipe[CBL_PIPE_NUM])
{
	return pipe(iPipe);
}

/*******************************************************************************
*   函 数   名：ReadPipe
*   功     能：读取管道数据
*   输入参数：iPipe:读管道文件描述符
*			  cReadBuffer:读取管道数据存放的缓存位置
*			  uiBufferLen:缓存长度
*   输出参数：无
*   返 回 值：CBL_PIPE_READ_BUFFER_VALID：buffer无效
*			  CBL_PIPE_READ_BUFFER_LEN_VALID：buffer长度无效
*			  0:管道关闭或管道中无数据
*			  >0:返回读取管道数据的长度
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/
INT32 ReadPipe(INT32 iRdPipe, CHAR *cReadBuffer, UINT32 uiBufferLen)
{
	INT32 iReadResult = 0;
	
	/* 参数校验 */
	if (CBL_NULL == cReadBuffer)
	{
		return CBL_PIPE_READ_BUFFER_VALID;
	}

	if (uiBufferLen <= 0)
	{
		return CBL_PIPE_READ_BUFFER_LEN_VALID;
	}

	/* 读取管道数据 */
	iReadResult = read(iRdPipe, cReadBuffer, uiBufferLen);
	
	return iReadResult;
}

/*******************************************************************************
*   函 数   名：ReadPipe
*   功     能：读取管道数据
*   输入参数：iPipe:读管道文件描述符
*			  cReadBuffer:读取管道数据存放的缓存位置
*			  uiBufferLen:缓存长度
*   输出参数：无
*   返 回 值：CBL_PIPE_READ_BUFFER_VALID：buffer无效
*			  CBL_PIPE_READ_BUFFER_LEN_VALID：buffer长度无效
*			  0:管道关闭或管道中无数据
*			  >0:返回读取管道数据的长度
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/
INT32 WritePipe(INT32 iWrPipe, CHAR *cWriteMsg, UINT32 uiWriteMsgLen)
{
	INT32 iWriteResult = 0;
	
	/* 参数校验 */
	if (CBL_NULL == cWriteMsg)
	{
		return CBL_PIPE_WRITE_BUFFER_VALID;
	}

	if (uiWriteMsgLen <= 0)
	{
		return CBL_PIPE_WRITE_BUFFER_LEN_VALID;
	}

	/* 读取管道数据 */
	iWriteResult = write(iWrPipe, cWriteMsg, uiWriteMsgLen);
	
	return iWriteResult;
}

/*******************************************************************************
*   函 数   名：ClosePipe
*   功     能：关闭管道
*   输入参数：iPipe:管道文件描述符
*   输出参数：无
*   返 回 值：无
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
VOID ClosePipe(INT32 iPipe)
{
	close(iPipe);
	
	return;
}

/*******************************************************************************
*   函 数   名：CreatePopen
*   功     能：创建管道
*   输入参数：cCommand:是一个在shell中可运行的命令字符串的指针
*             cMode:r/w;指示返回的文件描述符的打开模式
*   输出参数：无
*   返 回 值：文件指针
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
FILE *CreatePopen(const CHAR *pcCommand, const CHAR *pcMode)
{
    if (CBL_NULL == pcCommand || CBL_NULL == pcMode)
    {
        return CBL_NULL;
    }
    
	return popen(pcCommand, pcMode);
}

/*******************************************************************************
*   函 数   名：ReadPopen
*   功     能：从管道中读取数据
*   输入参数：pFile:文件描述符
*   输出参数：无
*   返 回 值：成功时返回第一个参数pcBuff;
*             在读字符时遇到end-of-file，则eof指示器被设置，如果还没读入任何字符就遇到这种情况，
*                 则buf保持原来的内容，返回NULL;
*             如果发生读入错误，error指示器被设置，返回NULL，buf的值可能被改变;
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
CHAR *ReadPopen(CHAR *pcBuff, INT32 iLen, FILE *pFile)
{
    if (CBL_NULL == pcBuff)
    {
        return CBL_NULL;
    }

    if (iLen <= 0)
    {
        return CBL_NULL;
    }
    
	return fgets(pcBuff, iLen, pFile);
}

/*******************************************************************************
*   函 数   名：ClosePclose
*   功     能：关闭管道
*   输入参数：pFile:文件描述符
*   输出参数：无
*   返 回 值：失败时返回-1
*   创建日期：018-7-18
*   修改历史：无
*******************************************************************************/
INT32 ClosePclose(FILE *pFile)
{
    if (CBL_NULL == pFile)
    {
        return CBL_PCLOSE_FILE_VALID;
    }
    
	return pclose(pFile);
}

