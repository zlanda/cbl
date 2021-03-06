/*******************************************************************************
*   文  件 名：Msqid.c
*   功     能：封装消息队列功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/

#include "Msqid.h"

//https://www.cnblogs.com/52php/p/5862114.html

/*******************************************************************************
*   函 数   名：CreateMsgQid
*   功     能：创建和访问一个消息队列
*   输入参数：iKey: 提供一个键来命名某个特定的消息队列
*             iMsgFlag: 是一个权限标志，表示消息队列的访问权限，它与文件的访问权限一样。
*               iMsgFlag可以与IPC_CREAT做或操作，表示当iKey所命名的消息队列不存在时创建
*               一个消息队列，如果iKey所命名的消息队列存在时，IPC_CREAT标志会被忽略，而
*               只返回一个标识符。
*   输出参数：无
*   返 回 值：成功时返回一个以iKey命名的消息队列的标识符（非零整数）
*             失败返回-1
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 CreateMsgQid(key_t iKey, INT32 iMsgFlag)
{
	return msgget(iKey, iMsgFlag);
}

/*******************************************************************************
*   函 数   名：SengMsgQid
*   功     能：该函数用来把消息队列添加到消息队列中
*   输入参数：iMsgId: 是由msgget函数返回的消息队列标识符
*             pvMsg: 是一个指向准备发送消息的指针，但是消息的数据结构却有一定的要求，
*               指针pvMsg所指向的消息结构一定要是以一个长整型成员变量开始的结构体，接收
*               函数将用这个成员来确定消息的类型。所以消息结构要定义成这样。
*               struct my_message {
*                   long int message_type;
*                   //The data you wish to transfer
*               }
*             iMsgSize:是pvMsg指向的消息的长度，注意是消息的长度，而不是整个结构体的长度，
*               也就是说iMsgSize是不包括长整型消息类型成员变量的长度
*             iMsgFlag:用于控制当前消息队列满或消息队列到达系统范围的限制时将要发生的事情。
*   输出参数：无
*   返 回 值：成功时，消息数据的一份副本将被放到消息队列中，并返回0
*             失败返回-1
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 SengMsgQid(INT32 iMsgId, const VOID *pvMsg, size_t iMsgSize, INT32 iMsgFlag)
{
	return msgsnd(iMsgId, pvMsg, iMsgSize, iMsgFlag);
}

/*******************************************************************************
*   函 数   名：RecvMsgQid
*   功     能：从一个消息队列获取消息
*   输入参数：iMsgId: 是由msgget函数返回的消息队列标识符
*             pvMsg: 是一个指向准备接收消息的指针，但是消息的数据结构却有一定的要求，
*               指针pvMsg所指向的消息结构一定要是以一个长整型成员变量开始的结构体，接收
*               函数将用这个成员来确定消息的类型。所以消息结构要定义成这样。
*               struct my_message {
*                   long int message_type;
*                   //The data you wish to transfer
*               }
*             iMsgSize:是pvMsg指向的消息的长度，注意是消息的长度，而不是整个结构体的长度，
*               也就是说iMsgSize是不包括长整型消息类型成员变量的长度
*             lMsgType:可以实现一种简单的接收优先级。如果lMsgType为0，就获取队列中的第一个消息。
*               如果他的值大于0，将获取具有相同消息类型的第一个消息。如果他小于0，就获取类型等于
*               或小于lMsgType的绝对值的第一个消息。
*             iMsgFlag:用于控制当队列中没有相应类型的消息可以接收时将发生的事情。
*   输出参数：无
*   返 回 值：成功时，该函数返回放到接收缓存区中的字节数，消息被复制到由pvMsg指向的
*               用户分配的缓存区中，然后删除消息队列中的对应消息。
*             失败返回-1
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 RecvMsgQid(INT32 iMsgId, VOID *pvMsg, size_t iMsgSize, LONG lMsgType, INT32 iMsgFlag)
{
	return msgrcv(iMsgId, pvMsg, iMsgSize, lMsgType, iMsgFlag);
}

/*******************************************************************************
*   函 数   名：ControlMsgQid
*   功     能：用来控制消息队列，它与共享内存的shmctl函数相似。
*   输入参数：iMsgId: 是由msgget函数返回的消息队列标识符
*             iCmd: 是将要采取的动作，它可以取3个值：
*               IPC_STAT:把msgid_ds结构中的数据设置为消息队列的当前关联值，即用消息队列
*                   的当前关联值覆盖msgid_ds的值。
*               IPC_SET:如果进程有足够的权限，就把消息队列的当前关联值设置为msgid_ds结构中给出的值
*               IPC_RMID:删除消息队列
*             pstBuffer:是指向msgid_ds结构的指针，它指向消息队列模式和访问权限的结构。msgid_ds结构至少包括以下成员:
*               struct msgid_ds
*               {
*                   uid_t shm_perm.uid;
*                   uid_t shm_perm.gid;
*                   mode_t shm_perm.mode;
*               };
*   输出参数：无
*   返 回 值：成功时返回0
*             失败返回-1
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 ControlMsgQid(INT32 iMsgId, INT32 iCmd, struct msqid_ds *pstBuffer)
{
	return msgctl(iMsgId, iCmd, pstBuffer);
}

