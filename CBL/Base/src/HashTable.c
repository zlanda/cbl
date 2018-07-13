/*******************************************************************************
*   文  件 名：Stack.c
*   功     能：实现哈希表功能
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-11
*   修改历史：无
*******************************************************************************/

#include "List.h"
#include "HashTable.h"

/*******************************************************************************
*   函 数   名：HashTableInit
*   功     能：哈希表初始化
*   输入参数：pstHashTable:哈希表头
*             uiHashTableLen:哈希表大小
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-11
*   修改历史：无
*******************************************************************************/
BOOL HashTableInit(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableLen)
{
    UINT32 uiHashIndex = 0;
    
    /* 参数校验 */
    if (CBL_NULL == pstHashTable)
    {
        return CBL_FALSE;
    }

    /* 如果哈希表大小小于等于0则直接返回 */
    if (uiHashTableLen <= 0)
    {
        return CBL_FALSE;
    }

    /* 初始化链表头 */
    for (uiHashIndex = 0; uiHashIndex < uiHashTableLen; uiHashIndex++)
    {
        ListHeadInit(&pstHashTable[uiHashIndex].stHashTableHead);
    }
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：HashTableElemMakeNode
*   功     能：哈希节点创建
*   输入参数：pstMsg:哈希节点承载消息
*   输出参数：无
*   返 回 值：CBL_NULL:创建节点失败
*             HASHTABLE_NODE_S *:创建节点成功
*   作     者：zhanxc
*   创建日期：2018-7-10
*   修改历史：无
*******************************************************************************/
static HASHTABLE_NODE_S *HashTableElemMakeNode(HASHTABLE_MSG_S *pstMsg)
{
    HASHTABLE_NODE_S *pstHashTableNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstMsg)
    {
        return CBL_NULL;
    }

    /* 哈希节点内存分配 */
    pstHashTableNode = (HASHTABLE_NODE_S *)malloc(sizeof(HASHTABLE_NODE_S));
    if (CBL_NULL == pstHashTableNode)
    {
        return CBL_NULL;
    }

    /* 先清空内存 */
    memset(pstHashTableNode, 0, sizeof(HASHTABLE_NODE_S));

    /* 哈希节点承载消息内存分配 */
    pstHashTableNode->pstHashTableMsg = (HASHTABLE_MSG_S *)malloc(sizeof(HASHTABLE_MSG_S));
    if (CBL_NULL == pstHashTableNode->pstHashTableMsg)
    {
        return CBL_NULL;
    }

    /* 清空承载消息内存 */
    memset(pstHashTableNode->pstHashTableMsg, 0, sizeof(HASHTABLE_MSG_S));

    /* 消息内存分配 */
    pstHashTableNode->pstHashTableMsg->pvMsg = (VOID *)malloc(pstMsg->uiMsgLen);
    if (CBL_NULL == pstHashTableNode->pstHashTableMsg->pvMsg)
    {
        return CBL_NULL;
    }

    /* 清空消息 */
    memset(pstHashTableNode->pstHashTableMsg->pvMsg, 0, pstMsg->uiMsgLen);

    /* 填充消息 */
    memcpy(pstHashTableNode->pstHashTableMsg->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen);
    pstHashTableNode->pstHashTableMsg->uiMsgLen = pstMsg->uiMsgLen;

    return pstHashTableNode;
}

/*******************************************************************************
*   函 数   名：HashTableHeadInsert
*   功     能：从哈希表某个索引从头位置处插入消息
*   输入参数：pstHashTable:哈希表头
*             uiHashTableIndex:哈希表索引
*             pstMsg:插入哈希表的消息
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-11
*   修改历史：无
*******************************************************************************/
BOOL HashTableHeadInsert(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex, HASHTABLE_MSG_S *pstMsg)
{
    HASHTABLE_NODE_S *pstInsertHashTableNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstHashTable)
    {
        return CBL_FALSE;
    }

    /* 如果哈希表大小小于等于0则直接返回，注意:哈希索引有效性由调用者保证 */
    if (uiHashTableIndex <= 0)
    {
        return CBL_FALSE;
    }

    /* 生成哈希消息节点 */
    pstInsertHashTableNode = HashTableElemMakeNode(pstMsg);
    if (CBL_NULL == pstInsertHashTableNode)
    {
        return CBL_FALSE;
    }

    /* 挂链 */
    ListAddHead(&pstInsertHashTableNode->stHashTableList, &pstHashTable[uiHashTableIndex].stHashTableHead);

    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：HashTableTailInsert
*   功     能：从哈希表某个索引从尾位置处插入消息
*   输入参数：pstHashTable:哈希表头
*             uiHashTableIndex:哈希表索引
*             pstMsg:插入哈希表的消息
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-11
*   修改历史：无
*******************************************************************************/
BOOL HashTableTailInsert(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex, HASHTABLE_MSG_S *pstMsg)
{
    HASHTABLE_NODE_S *pstInsertHashTableNode = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstHashTable)
    {
        return CBL_FALSE;
    }

    /* 如果哈希表大小小于等于0则直接返回，注意:哈希索引有效性由调用者保证 */
    if (uiHashTableIndex <= 0)
    {
        return CBL_FALSE;
    }

    /* 生成哈希消息节点 */
    pstInsertHashTableNode = HashTableElemMakeNode(pstMsg);
    if (CBL_NULL == pstInsertHashTableNode)
    {
        return CBL_FALSE;
    }

    /* 挂链 */
    ListAddTail(&pstInsertHashTableNode->stHashTableList, &pstHashTable[uiHashTableIndex].stHashTableHead);

    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：HashTableDel
*   功     能：从哈希表中删除某个节点
*   输入参数：pstHashTable:哈希表头
*             uiHashTableIndex:哈希表索引
*             pstMsg:插入哈希表的消息
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-11
*   修改历史：无
*******************************************************************************/
BOOL HashTableDel(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex, HASHTABLE_MSG_S *pstMsg)
{
    HASHTABLE_NODE_S *pstDelHashTableNode = CBL_NULL;
    LIST_HEAD_S *pstList = CBL_NULL;
    LIST_HEAD_S *pstListHead = CBL_NULL;
    BOOL bRet = CBL_FALSE;
    
    /* 参数校验 */
    if (CBL_NULL == pstHashTable)
    {
        return CBL_FALSE;
    }

    /* 如果哈希表大小小于等于0则直接返回，注意:哈希索引有效性由调用者保证 */
    if (uiHashTableIndex <= 0)
    {
        return CBL_FALSE;
    }

    /* 获取到链表头 */
    pstListHead = &pstHashTable[uiHashTableIndex].stHashTableHead;

    /* 检查哈希表索引位置处链表是否为空，为空则直接返回 */
    if (ListIsEmpty(pstListHead))
    {
        return CBL_FALSE;
    }

    /* 轮询 */
    for (pstList = pstListHead->pstNext; pstList != pstListHead; pstList = pstList->pstNext)
    {
        /* 取得待删除的哈希表节点 */
        pstDelHashTableNode = CONTAINER_IN(pstList, HASHTABLE_NODE_S, stHashTableList);
        if ((pstDelHashTableNode->pstHashTableMsg->uiMsgLen == pstMsg->uiMsgLen) &&
            (0 == memcmp(pstDelHashTableNode->pstHashTableMsg->pvMsg, pstMsg->pvMsg, 
                         pstDelHashTableNode->pstHashTableMsg->uiMsgLen)))
        {
            /* 消息完全匹配则开始删除节点 */
            ListDel(&pstDelHashTableNode->stHashTableList);

            /* 释放内存 */
            free(pstDelHashTableNode->pstHashTableMsg->pvMsg);
            free(pstDelHashTableNode->pstHashTableMsg);
            free(pstDelHashTableNode);

            /* 删除完成跳出循环 */
            bRet = CBL_TRUE;
            break;
        }
    }

    return bRet;
}

/*******************************************************************************
*   函 数   名：HashTableTraversal
*   功     能：遍历哈希表
*   输入参数：pstHashTable:哈希表头
*             uiHashTableIndex:哈希表索引
*             pstMsg:插入哈希表的消息
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-11
*   修改历史：无
*******************************************************************************/
VOID HashTableTraversal(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex)
{
    HASHTABLE_NODE_S *pstHashTableNode = CBL_NULL;
    LIST_HEAD_S *pstList = CBL_NULL;
    LIST_HEAD_S *pstListHead = CBL_NULL;
    
    /* 参数校验 */
    if (CBL_NULL == pstHashTable)
    {
        return;
    }

    /* 如果哈希表大小小于等于0则直接返回，注意:哈希索引有效性由调用者保证 */
    if (uiHashTableIndex <= 0)
    {
        return;
    }

    /* 获取到链表头 */
    pstListHead = &pstHashTable[uiHashTableIndex].stHashTableHead;

    /* 检查哈希表索引位置处链表是否为空，为空则直接返回 */
    if (ListIsEmpty(pstListHead))
    {
        return;
    }

    /* 轮询 */
    for (pstList = pstListHead->pstNext; pstList != pstListHead; pstList = pstList->pstNext)
    {
        /* 取得待删除的哈希表节点 */
        pstHashTableNode = CONTAINER_IN(pstList, HASHTABLE_NODE_S, stHashTableList);

        /* 强校验一下安全性 */
        if (CBL_NULL == pstHashTableNode)
        {
            continue;
        }

        /* 显示消息 */
        printf("Message:%s\r\n", (UINT8 *)pstHashTableNode->pstHashTableMsg->pvMsg);
    }
    
    return;
}

