/*******************************************************************************
*   文  件 名：Queue.c
*   功     能：实现队列功能
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#include "Queue.h"

/*******************************************************************************
*   函 数   名：QueueInit
*   功     能：队列初始化
*   输入参数：pstQueue:待初始化的队列头
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL QueueInit(QUEUE_HEAD_S *pstQueue)
{
    /* 参数校验 */
    if (CBL_NULL == pstQueue)
    {
        return CBL_FALSE;
    }
    
    pstQueue->pstFront = CBL_NULL;
    pstQueue->pstRear = CBL_NULL;
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：QueueIsEmpty
*   功     能：队列是否为空
*   输入参数：pstQueue:队列头指针
*   输出参数：无
*   返 回 值：CBL_FALSE:队列不为空
*             CBL_TRUE:队列为空
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL QueueIsEmpty(QUEUE_HEAD_S *pstQueue)
{
    /* 参数校验 */
    if (CBL_NULL == pstQueue)
    {
        return CBL_TRUE;
    }
    
    if (CBL_NULL == pstQueue->pstFront)
    {
        return CBL_TRUE;
    }
    
    return CBL_FALSE;
}

/*******************************************************************************
*   函 数   名：QueueElemMakeNode
*   功     能：消息成员节点创建
*   输入参数：pstMsg:入队列的消息
*   输出参数：无
*   返 回 值：CBL_NULL:创建节点失败
*             QUEUE_NODE_S *:创建节点成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
static QUEUE_NODE_S *QueueElemMakeNode(QUEUE_MSG_S *pstMsg)
{
    QUEUE_NODE_S *pstQueueNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstMsg)
    {
        return CBL_NULL;
    }

    /* 参数校验通过后则为消息节点分配内存 */
    pstQueueNode = (QUEUE_NODE_S *)malloc(sizeof(QUEUE_NODE_S));
    if (CBL_NULL == pstQueueNode)
    {
        return CBL_NULL;
    }

    /* 先清空内存 */
    memset(pstQueueNode, 0, sizeof(QUEUE_NODE_S));

    /* 消息内存申请 */
    pstQueueNode->pvMsg = (VOID *)malloc(pstMsg->uiMsgLen);
    if (CBL_NULL == pstQueueNode->pvMsg)
    {
        return CBL_NULL;
    }

    /* 清空消息内存 */
    memset(pstQueueNode->pvMsg, 0, pstMsg->uiMsgLen);

    /* 填充消息 */
    memcpy(pstQueueNode->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen);
    pstQueueNode->uiMsgLen = pstMsg->uiMsgLen;

    /* 强制消息的前驱后继为空 */
    pstQueueNode->pstNext = CBL_NULL;
    pstQueueNode->pstPrev = CBL_NULL;

    return pstQueueNode;
}

/*******************************************************************************
*   函 数   名：Enqueue
*   功     能：入队
*   输入参数：pstQueue:队列头指针
*             pstMsg:入队列的消息
*   输出参数：无
*   返 回 值：CBL_FALSE:入队失败
*             CBL_TRUE:入队成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL Enqueue(QUEUE_HEAD_S *pstQueue, QUEUE_MSG_S *pstMsg)
{
    QUEUE_NODE_S *pstQueueInsertNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstQueue || CBL_NULL == pstMsg)
    {
        return CBL_FALSE;
    }

    /* 创建消息队列节点 */
    pstQueueInsertNode = QueueElemMakeNode(pstMsg);
    if (CBL_NULL == pstQueueInsertNode)
    {
        return CBL_FALSE;
    }
    
    /* 检查队列是否为空 */
    if (QueueIsEmpty(pstQueue))
    {
        /* 对头指针指向刚创建的队列节点,队尾初始化时已经设置了为空 */
        pstQueue->pstFront = pstQueueInsertNode;
    }
    else
    {
        /* 先建立队列节点关系 */
        if (CBL_NULL == pstQueue->pstRear)
        {
            pstQueueInsertNode->pstPrev = pstQueue->pstFront;
            pstQueue->pstFront->pstNext = pstQueueInsertNode;
        }
        else
        {
            pstQueueInsertNode->pstPrev = pstQueue->pstRear;
            pstQueue->pstRear->pstNext = pstQueueInsertNode;
        }
        
        /* 队列不为空则将队尾指向刚创建的队列节点 */
        pstQueue->pstRear = pstQueueInsertNode;
    }
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：Dequeue
*   功     能：出队
*   输入参数：pstQueue:队列头指针
*   输出参数：pstMsg:出队的消息
*   返 回 值：CBL_FALSE:队列不为空
*             CBL_TRUE:队列为空
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL Dequeue(QUEUE_HEAD_S *pstQueue, QUEUE_MSG_S *pstMsg)
{
    QUEUE_NODE_S *pstDeQueueNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstQueue || CBL_NULL == pstMsg || CBL_NULL == pstMsg->pvMsg)
    {
        return CBL_FALSE;
    }

    /* 如果队列是空的则直接返回 */
    if (QueueIsEmpty(pstQueue))
    {
        return CBL_FALSE;
    }
    
    /* 出队从对头开始出，将消息拷贝到pstMsg，软后释放资源后挪动队列头位置 */
    pstDeQueueNode = pstQueue->pstFront;

    /* 填充输出消息 */
    memcpy(pstMsg->pvMsg, pstDeQueueNode->pvMsg, pstDeQueueNode->uiMsgLen);
    pstMsg->uiMsgLen = pstDeQueueNode->uiMsgLen;

    /* 队列头挪动位置 */
    pstQueue->pstFront = pstQueue->pstFront->pstNext;

    /* 释放出队节点资源 */
    free(pstDeQueueNode->pvMsg);
    free(pstDeQueueNode);
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：FlushQueue
*   功     能：清空队列
*   输入参数：pstQueue:队列头指针
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
VOID FlushQueue(QUEUE_HEAD_S *pstQueue)
{
    QUEUE_NODE_S *pstDeQueueNode = CBL_NULL;
    QUEUE_NODE_S *pstFrontNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstQueue)
    {
        return;
    }

    /* 如果队列是空的则直接返回 */
    if (QueueIsEmpty(pstQueue))
    {
        return;
    }
    
    pstFrontNode = pstQueue->pstFront;
    while (CBL_NULL != pstFrontNode)
    {
        /* 待删除节点 */
        pstDeQueueNode = pstFrontNode;
        
        /* 队列新的头位置 */
        pstFrontNode = pstFrontNode->pstNext;
        
        /* 队列头移动 */
        pstQueue->pstFront = pstFrontNode;
        
        /* 释放资源 */
        free(pstDeQueueNode->pvMsg);
        free(pstDeQueueNode);
    }
    
    return;
}

/*******************************************************************************
*   函 数   名：TraversalFrontQueue
*   功     能：从队列头开始遍历队列
*   输入参数：pstQueue:队列头指针
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
VOID TraversalFrontQueue(QUEUE_HEAD_S *pstQueue)
{
    QUEUE_NODE_S *pstTraversalNode = CBL_NULL;
    UINT32 uiNodeCnt = 0;
    UINT32 uiMsgCnt = 0;

    /* 参数校验 */
    if (CBL_NULL == pstQueue)
    {
        return;
    }

    /* 检查队列是否为空 */
    if (QueueIsEmpty(pstQueue))
    {
        return;
    }

    printf("=======================================\r\n");
    
    pstTraversalNode = pstQueue->pstFront;
    while (CBL_NULL != pstTraversalNode)
    {
        uiNodeCnt++;
        printf("No%d:", uiNodeCnt);
        for (uiMsgCnt = 0; uiMsgCnt < pstTraversalNode->uiMsgLen; uiMsgCnt++)
        {
            printf("%x ", *((UINT8 *)(pstTraversalNode->pvMsg) + uiMsgCnt));
        }
        printf("\r\n");
        pstTraversalNode = pstTraversalNode->pstNext;
    }
    
    printf("=======================================\r\n");

    return;
}

/*******************************************************************************
*   函 数   名：TraversalRearQueue
*   功     能：从队列尾开始遍历队列
*   输入参数：pstQueue:队列头指针
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
VOID TraversalRearQueue(QUEUE_HEAD_S *pstQueue)
{
    QUEUE_NODE_S *pstTraversalNode = CBL_NULL;
    UINT32 uiNodeCnt = 0;
    UINT32 uiMsgCnt = 0;

    /* 参数校验 */
    if (CBL_NULL == pstQueue)
    {
        return;
    }

    /* 检查队列是否为空 */
    if (QueueIsEmpty(pstQueue))
    {
        return;
    }

    printf("=======================================\r\n");
    
    pstTraversalNode = pstQueue->pstRear;
    while (CBL_NULL != pstTraversalNode)
    {
        uiNodeCnt++;
        printf("No%d:", uiNodeCnt);
        for (uiMsgCnt = 0; uiMsgCnt < pstTraversalNode->uiMsgLen; uiMsgCnt++)
        {
            printf("%x ", *((UINT8 *)(pstTraversalNode->pvMsg) + uiMsgCnt));
        }
        printf("\r\n");
        pstTraversalNode = pstTraversalNode->pstPrev;
    }
    
    printf("=======================================\r\n");

    return;
}

