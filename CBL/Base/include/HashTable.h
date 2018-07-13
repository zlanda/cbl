/*******************************************************************************
*   文  件 名：Stack.h
*   功     能：哈希表对外接口
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-11
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_BASE_HASH_TABLE_H_
#define  _CBL_BASE_HASH_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/*
目的：实现哈希链表
*/
/* 外部入哈希表消息结构 */
typedef struct HashTable_Msg_Tag
{
    VOID *pvMsg;                    /* 消息 */
    UINT32 uiMsgLen;                /* 消息长度 */
} HASHTABLE_MSG_S;

/* 哈希表节点结构 */
typedef struct HashTable_Node_Tag
{
    HASHTABLE_MSG_S *pstHashTableMsg;   /* 承载消息 */
    LIST_HEAD_S stHashTableList;        /* 哈希链表 */
} HASHTABLE_NODE_S;

/* 哈希表头结构 */
typedef struct HashTable_Head_Tag
{
    LIST_HEAD_S stHashTableHead;      /* 哈希表头 */
} HASHTABLE_HEAD_S;

/* 哈希表初始化 */
BOOL HashTableInit(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableLen);

/* 从哈希表某个索引从头位置处插入消息 */
BOOL HashTableHeadInsert(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex, HASHTABLE_MSG_S *pstMsg);

/* 从哈希表某个索引从尾位置处插入消息 */
BOOL HashTableTailInsert(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex, HASHTABLE_MSG_S *pstMsg);

/* 从哈希表中删除某个节点 */
BOOL HashTableDel(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex, HASHTABLE_MSG_S *pstMsg);

/* 遍历哈希表 */
VOID HashTableTraversal(HASHTABLE_HEAD_S *pstHashTable, UINT32 uiHashTableIndex);


#ifdef __cplusplus
}
#endif

#endif

