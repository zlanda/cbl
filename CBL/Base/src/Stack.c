/*******************************************************************************
*   文  件 名：Stack.c
*   功     能：实现栈功能
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#include "Stack.h"

/*******************************************************************************
*   函 数   名：StackInit
*   功     能：栈初始化
*   输入参数：pstStack:待初始化的队列头，内存外部保证有效
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL StackInit(STACK_HEAD_S *pstStack)
{
    /* 参数校验 */
    if (CBL_NULL == pstStack)
    {
        return CBL_FALSE;
    }
    
    pstStack->pstTop = CBL_NULL;
    pstStack->pstTail = CBL_NULL;
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：StackIsEmpty
*   功     能：栈是否为空
*   输入参数：pstStack:栈指针
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL StackIsEmpty(STACK_HEAD_S *pstStack)
{
    /* 参数校验 */
    if (CBL_NULL == pstStack)
    {
        return CBL_TRUE;
    }
    
    if (pstStack->pstTop == pstStack->pstTail)
    {
        return CBL_TRUE;
    }
    
    return CBL_FALSE;
}

/*******************************************************************************
*   函 数   名：StackElemMakeNode
*   功     能：消息成员节点创建
*   输入参数：pstMsg:入栈的消息
*   输出参数：无
*   返 回 值：CBL_NULL:创建节点失败
*             QUEUE_NODE_S *:创建节点成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
static STACK_NODE_S *StackElemMakeNode(STACK_MSG_S *pstMsg)
{
    STACK_NODE_S *pstStackNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstMsg)
    {
        return CBL_NULL;
    }

    /* 参数校验通过后则为消息节点分配内存 */
    pstStackNode = (STACK_NODE_S *)malloc(sizeof(STACK_NODE_S));
    if (CBL_NULL == pstStackNode)
    {
        return CBL_NULL;
    }

    /* 先清空内存 */
    memset(pstStackNode, 0, sizeof(STACK_NODE_S));

    /* 消息内存申请 */
    pstStackNode->pvMsg = (VOID *)malloc(pstMsg->uiMsgLen);
    if (CBL_NULL == pstStackNode->pvMsg)
    {
        return CBL_NULL;
    }

    /* 清空消息内存 */
    memset(pstStackNode->pvMsg, 0, pstMsg->uiMsgLen);

    /* 填充消息 */
    memcpy(pstStackNode->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen);
    pstStackNode->uiMsgLen = pstMsg->uiMsgLen;

    /* 强制消息的前驱后继为空 */
    pstStackNode->pstNext = CBL_NULL;

    return pstStackNode;
}

/*******************************************************************************
*   函 数   名：StackPush
*   功     能：压栈
*   输入参数：pstStack:栈指针
*             pstMsg:待压栈消息
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL StackPush(STACK_HEAD_S *pstStack, STACK_MSG_S *pstMsg)
{
    STACK_NODE_S *pstStackPushNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstStack || CBL_NULL == pstMsg)
    {
        return CBL_FALSE;
    }

    /* 创建消息队列节点 */
    pstStackPushNode = StackElemMakeNode(pstMsg);
    if (CBL_NULL == pstStackPushNode)
    {
        return CBL_FALSE;
    }
    
    /* 检查队列是否为空 */
    if (StackIsEmpty(pstStack))
    {
        /* 将栈底指向当前创建的节点，此时栈顶为空 */
        pstStack->pstTail = pstStackPushNode;

        /* 栈顶的意义不是很大 */
        pstStack->pstTop = pstStack->pstTail->pstNext;
    }
    else
    {
        /* 将当前创建的节点的后继指向当前栈底 */
        pstStackPushNode->pstNext = pstStack->pstTail;
        
        /* 当前栈底移动到新位置 */
        pstStack->pstTail = pstStackPushNode;
    }
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：StackPop
*   功     能：出栈
*   输入参数：pstStack:栈指针
*   输出参数：pstMsg:出栈消息
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
BOOL StackPop(STACK_HEAD_S *pstStack, STACK_MSG_S *pstMsg)
{
    STACK_NODE_S *pstStackPopNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstStack || CBL_NULL == pstMsg || CBL_NULL == pstMsg->pvMsg)
    {
        return CBL_TRUE;
    }

    /* 如果栈为空则直接返回 */
    if (StackIsEmpty(pstStack))
    {
        return CBL_TRUE;
    }

    /* 获取栈底 */
    pstStackPopNode = pstStack->pstTail;

    /* 填充出栈消息 */
    memcpy(pstMsg->pvMsg, pstStackPopNode->pvMsg, pstStackPopNode->uiMsgLen);
    pstMsg->uiMsgLen = pstStackPopNode->uiMsgLen;

    /* 移动栈底位置 */
    pstStack->pstTail = pstStack->pstTail->pstNext;

    /* 释放资源 */
    free(pstStackPopNode->pvMsg);
    free(pstStackPopNode);
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：FlushStack
*   功     能：清空栈
*   输入参数：pstStack:栈指针
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
VOID FlushStack(STACK_HEAD_S *pstStack)
{
    STACK_NODE_S *pstStackPopNode = CBL_NULL;
    STACK_NODE_S *pstTailNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstStack)
    {
        return;
    }

    /* 如果栈是空的则直接返回 */
    if (StackIsEmpty(pstStack))
    {
        return;
    }
    
    pstTailNode = pstStack->pstTail;
    while (CBL_NULL != pstTailNode)
    {
        /* 待删除节点 */
        pstStackPopNode = pstTailNode;
        
        /* 栈底新的位置 */
        pstTailNode = pstTailNode->pstNext;
        
        /* 栈底移动 */
        pstStack->pstTail = pstTailNode;
        
        /* 释放资源 */
        free(pstStackPopNode->pvMsg);
        free(pstStackPopNode);
    }
    
    return;
}

/*******************************************************************************
*   函 数   名：StackTraversal
*   功     能：遍历栈
*   输入参数：pstStack:栈指针
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
VOID StackTraversal(STACK_HEAD_S *pstStack)
{
    STACK_NODE_S *pstTraversalNode = CBL_NULL;
    UINT32 uiNodeCnt = 0;
    UINT32 uiMsgCnt = 0;

    /* 参数校验 */
    if (CBL_NULL == pstStack)
    {
        return;
    }

    /* 检查栈是否为空 */
    if (StackIsEmpty(pstStack))
    {
        return;
    }

    printf("=======================================\r\n");
    
    pstTraversalNode = pstStack->pstTail;
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


