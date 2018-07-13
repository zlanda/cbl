/*******************************************************************************
*   文  件 名：Queue.h
*   功     能：队列对外接口
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_BASE_QUEUE_H_
#define  _CBL_BASE_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/*
目的：想做一个队列，这个队列就像现实中的排队一样，队列没有大小，只要内存足够
      就始终可以入队列，因此采用链表定义队列。
      并且外部传入的队列数据都将在队列中存放，外部申请的内存由外部负责释放。
      并且在队列中可以方便的前后访问
*/

/* 外部入队列消息结构 */
typedef struct Queue_Msg_Tag
{
    VOID *pvMsg;                        /* 入队列的消息 */
    UINT32 uiMsgLen;                    /* 消息的长度 */
} QUEUE_MSG_S;

/* 定义队列节点结构 */
typedef struct Queue_Node_Tag
{
    VOID *pvMsg;                        /* 存放外部传入的消息 */
    UINT32 uiMsgLen;                    /* 存放数据长度 */
    struct Queue_Node_Tag *pstNext;     /* 后继，记录队列NODE之间的关系 */
    struct Queue_Node_Tag *pstPrev;     /* 前驱，记录队列NODE之间的关系 */
} QUEUE_NODE_S;

/* 定义队列头结构 */
typedef struct Queue_Head_Tag
{
    QUEUE_NODE_S *pstFront;     /* 队列头指针 */
    QUEUE_NODE_S *pstRear;      /* 队列尾指针 */
} QUEUE_HEAD_S;

/* 对外接口 */
/* 队列初始化 */
BOOL QueueInit(QUEUE_HEAD_S *pstQueue);

/* 队列是否为空 */
BOOL QueueIsEmpty(QUEUE_HEAD_S *pstQueue);

/* 入队 */
BOOL Enqueue(QUEUE_HEAD_S *pstQueue, QUEUE_MSG_S *pstMsg);

/* 出队 */
BOOL Dequeue(QUEUE_HEAD_S *pstQueue, QUEUE_MSG_S *pstMsg);

/* 清空队列 */
VOID FlushQueue(QUEUE_HEAD_S *pstQueue);

/* 从队头开始遍历队列 */
VOID TraversalFrontQueue(QUEUE_HEAD_S *pstQueue);

/* 从队尾开始遍历队列 */
VOID TraversalRearQueue(QUEUE_HEAD_S *pstQueue);

#ifdef __cplusplus
}
#endif

#endif

