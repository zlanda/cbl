/*******************************************************************************
*   文  件 名：Stack.h
*   功     能：栈对外接口
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_BASE_STACK_H_
#define  _CBL_BASE_STACK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/*
目的：用链表实现栈
*/

/* 外部入栈消息结构 */
typedef struct Stack_Msg_Tag
{
    VOID *pvMsg;                        /* 入栈的消息 */
    UINT32 uiMsgLen;                    /* 消息的长度 */
} STACK_MSG_S;

/* 定义栈节点结构 */
typedef struct Stack_Node_Tag
{
    VOID *pvMsg;                        /* 存放外部传入的消息 */
    UINT32 uiMsgLen;                    /* 存放数据长度 */
    struct Stack_Node_Tag *pstNext;     /* 后继，记录队列NODE之间的关系 */
} STACK_NODE_S;

/* 栈头结构 */
typedef struct Stack_Head_Tag
{
    STACK_NODE_S *pstTop;               /* 栈顶指针 */
    STACK_NODE_S *pstTail;              /* 栈底指针 */
} STACK_HEAD_S;

/* 栈初始化 */
BOOL StackInit(STACK_HEAD_S *pstStack);

/* 栈是否为空 */
BOOL StackIsEmpty(STACK_HEAD_S *pstStack);

/* 入栈 */
BOOL StackPush(STACK_HEAD_S *pstStack, STACK_MSG_S *pstMsg);

/* 出栈 */
BOOL StackPop(STACK_HEAD_S *pstStack, STACK_MSG_S *pstMsg);

/* 清空栈 */
VOID FlushStack(STACK_HEAD_S *pstStack);

/* 遍历栈 */
VOID StackTraversal(STACK_HEAD_S *pstStack);

#ifdef __cplusplus
}
#endif

#endif

