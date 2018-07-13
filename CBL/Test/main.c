#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Queue.h"
#include "Stack.h"
#include "HashTable.h"
#include "Btree.h"

#define LIST_TEST                               0
#define QUEUE_TEST                              0
#define STACK_TEST                              0
#define HASHTABLE_TEST                          0
#define BTREE_TESE                              1

#if LIST_TEST
typedef struct LIST_NODE_Tag
{
    UINT32 uiNodeValue;
    LIST_HEAD_S stList;
} LIST_NODE_S;

UINT32 ListShow(LIST_HEAD_S *pstList)
{
    LIST_NODE_S *pstListNode = CBL_NULL;
    
    printf("Show list entry.\r\n");

    pstListNode = CONTAINER_IN(pstList, LIST_NODE_S, stList);

    printf("NodeValue=%d\r\n", pstListNode->uiNodeValue);

    return 0;
}
#endif

#if QUEUE_TEST
#define QUEUE_TEST_STR          "qwerty1234567890zxcvbnm"
#define QUEUE_TEST_STR1         "aaaaaaaaaaaaaaaaaaaaaaa"
#define QUEUE_TEST_STR2         "bbbbbbbbbbbbbbbbbbbbbbb"
#define QUEUE_TEST_STR3         "ccccccccccccccccccccccc"
#define QUEUE_TEST_STR4         "ddddddddddddddddddddddd"
#endif

#if STACK_TEST
#define STACK_TEST_STR          "qwerty1234567890zxcvbnm"
#define STACK_TEST_STR1         "aaaaaaaaaaaaaaaaaaaaaaa"
#define STACK_TEST_STR2         "bbbbbbbbbbbbbbbbbbbbbbb"
#define STACK_TEST_STR3         "ccccccccccccccccccccccc"
#define STACK_TEST_STR4         "ddddddddddddddddddddddd"
#endif

#if HASHTABLE_TEST
#define HASH_TEST_STR           "qwerty1234567890zxcvbnm"
#define HASH_TEST_STR1          "aaaaaaaaaaaaaaaaaaaaaaa"
#define HASH_TEST_STR2          "bbbbbbbbbbbbbbbbbbbbbbb"
#define HASH_TEST_STR3          "ccccccccccccccccccccccc"
#define HASH_TEST_STR4          "ddddddddddddddddddddddd"

#define HASHTABLE_SIZE          6

HASHTABLE_HEAD_S g_stHashTable[HASHTABLE_SIZE];

#endif

#if BTREE_TESE
BTREE_S g_stBTree;

#endif

int main()
{
#if LIST_TEST
    LIST_HEAD_S stListHead;
    LIST_NODE_S stNewNode1 = {0};
    LIST_NODE_S stNewNode2 = {0};
    LIST_NODE_S stNewNode3 = {0};
    
    printf("Enter List test.\r\n");

    printf("Init the List.\r\n");
    ListHeadInit(&stListHead);

    stNewNode1.uiNodeValue = 10;
    ListAddTail(&stNewNode1.stList, &stListHead);

    stNewNode2.uiNodeValue = 100;
    ListAddHead(&stNewNode2.stList, &stListHead);

    stNewNode3.uiNodeValue = 1000;
    ListAddTail(&stNewNode3.stList, &stListHead);

    ListForEach(&stListHead, ListShow);
    printf("=============================================\r\n");

    ListDel(&stNewNode2.stList);
    ListForEach(&stListHead, ListShow);
    printf("=============================================\r\n");

    ListDel(&stNewNode1.stList);
    ListForEach(&stListHead, ListShow);
    printf("=============================================\r\n");

    ListDel(&stNewNode3.stList);
    ListForEach(&stListHead, ListShow);
    printf("=============================================\r\n");

    printf("Quite List test.\r\n");
#endif

#if QUEUE_TEST
    QUEUE_HEAD_S stQueue;
    QUEUE_MSG_S stMsg;
    UINT32 uiTest = 0x12345678;

    QueueInit(&stQueue);
    stMsg.pvMsg = (UINT8 *)malloc(100);
    if (CBL_NULL == stMsg.pvMsg)
    {
        return -1;
    }

    printf("Enqueue......\r\n");
    
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, QUEUE_TEST_STR, strlen(QUEUE_TEST_STR) + 1);
    stMsg.uiMsgLen = strlen(QUEUE_TEST_STR) + 1;
    printf("Enqueue:%s\r\n", stMsg.pvMsg);
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, &uiTest, 4);
    stMsg.uiMsgLen = sizeof(UINT32);
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, QUEUE_TEST_STR1, strlen(QUEUE_TEST_STR1) + 1);
    stMsg.uiMsgLen = strlen(QUEUE_TEST_STR1) + 1;
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, QUEUE_TEST_STR2, strlen(QUEUE_TEST_STR2) + 1);
    stMsg.uiMsgLen = strlen(QUEUE_TEST_STR2) + 1;
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, QUEUE_TEST_STR3, strlen(QUEUE_TEST_STR3) + 1);
    stMsg.uiMsgLen = strlen(QUEUE_TEST_STR3) + 1;
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, QUEUE_TEST_STR4, strlen(QUEUE_TEST_STR4) + 1);
    stMsg.uiMsgLen = strlen(QUEUE_TEST_STR4) + 1;
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);
    TraversalRearQueue(&stQueue);

    printf("Dequeue......\r\n");
    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue1:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue2:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue3:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue4:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue5:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue7:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue7:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

    printf("Enqueue......\r\n");
    
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, QUEUE_TEST_STR, strlen(QUEUE_TEST_STR) + 1);
    stMsg.uiMsgLen = strlen(QUEUE_TEST_STR) + 1;
    printf("Enqueue:%s\r\n", stMsg.pvMsg);
    /* 插入队列 */
    Enqueue(&stQueue, &stMsg);

    /* 出队 */
    memset(stMsg.pvMsg, 0, 100);
    Dequeue(&stQueue, &stMsg);
    printf("Dequeue8:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    TraversalFrontQueue(&stQueue);

#if 0
    /* Flush队列 */
    FlushQueue(&stQueue);

    printf("Flush......\r\n");
    /* 遍历 */
    TraversalFrontQueue(&stQueue);
#endif
    
#endif

#if STACK_TEST
    STACK_HEAD_S stStack;
    STACK_MSG_S stMsg;
    UINT32 uiTest = 0x12345678;

    StackInit(&stStack);
    stMsg.pvMsg = (UINT8 *)malloc(100);
    if (CBL_NULL == stMsg.pvMsg)
    {
        return -1;
    }

    printf("Push......\r\n");
    
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, STACK_TEST_STR, strlen(STACK_TEST_STR) + 1);
    stMsg.uiMsgLen = strlen(STACK_TEST_STR) + 1;
    printf("Push:%s\r\n", stMsg.pvMsg);
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, &uiTest, 4);
    stMsg.uiMsgLen = sizeof(UINT32);
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, STACK_TEST_STR1, strlen(STACK_TEST_STR1) + 1);
    stMsg.uiMsgLen = strlen(STACK_TEST_STR1) + 1;
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, STACK_TEST_STR2, strlen(STACK_TEST_STR2) + 1);
    stMsg.uiMsgLen = strlen(STACK_TEST_STR2) + 1;
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, STACK_TEST_STR3, strlen(STACK_TEST_STR3) + 1);
    stMsg.uiMsgLen = strlen(STACK_TEST_STR3) + 1;
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, STACK_TEST_STR4, strlen(STACK_TEST_STR4) + 1);
    stMsg.uiMsgLen = strlen(STACK_TEST_STR4) + 1;
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    printf("Pop......\r\n");
    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop1:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop2:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop3:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop4:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop5:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop6:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop7:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);

    printf("Push......\r\n");
    
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, STACK_TEST_STR, strlen(STACK_TEST_STR) + 1);
    stMsg.uiMsgLen = strlen(STACK_TEST_STR) + 1;
    printf("Push:%s\r\n", stMsg.pvMsg);
    /* 入栈 */
    StackPush(&stStack, &stMsg);

    /* 出栈 */
    memset(stMsg.pvMsg, 0, 100);
    StackPop(&stStack, &stMsg);
    printf("Pop8:%s\r\n", stMsg.pvMsg);

    /* 遍历 */
    StackTraversal(&stStack);
    
#if 0    
    /* Flush */
    FlushStack(&stStack);

    printf("Flush......\r\n");
    /* 遍历 */
    StackTraversal(&stStack);
#endif

#endif

#if HASHTABLE_TEST
    HASHTABLE_MSG_S stMsg;

    stMsg.pvMsg = (UINT8 *)malloc(100);
    if (CBL_NULL == stMsg.pvMsg)
    {
        return -1;
    }
    
    HashTableInit(g_stHashTable, HASHTABLE_SIZE);

    printf("Insert......\r\n");
    /* 插入哈希表 */
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, HASH_TEST_STR, strlen(HASH_TEST_STR) + 1);
    stMsg.uiMsgLen = strlen(HASH_TEST_STR) + 1;
    HashTableHeadInsert(g_stHashTable, 1, &stMsg);

    /* 插入哈希表 */
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, HASH_TEST_STR1, strlen(HASH_TEST_STR1) + 1);
    stMsg.uiMsgLen = strlen(HASH_TEST_STR1) + 1;
    HashTableHeadInsert(g_stHashTable, 1, &stMsg);

    /* 插入哈希表 */
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, HASH_TEST_STR2, strlen(HASH_TEST_STR2) + 1);
    stMsg.uiMsgLen = strlen(HASH_TEST_STR2) + 1;
    HashTableHeadInsert(g_stHashTable, 1, &stMsg);

    /* 插入哈希表 */
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, HASH_TEST_STR3, strlen(HASH_TEST_STR3) + 1);
    stMsg.uiMsgLen = strlen(HASH_TEST_STR3) + 1;
    HashTableHeadInsert(g_stHashTable, 1, &stMsg);

    /* 插入哈希表 */
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, HASH_TEST_STR4, strlen(HASH_TEST_STR4) + 1);
    stMsg.uiMsgLen = strlen(HASH_TEST_STR4) + 1;
    HashTableHeadInsert(g_stHashTable, 1, &stMsg);

    /* 显示 */
    HashTableTraversal(g_stHashTable, 1);

    printf("Delete......\r\n");
    /* 删除 */
    memset(stMsg.pvMsg, 0, 100);
    memcpy(stMsg.pvMsg, HASH_TEST_STR3, strlen(HASH_TEST_STR3) + 1);
    stMsg.uiMsgLen = strlen(HASH_TEST_STR3) + 1;
    HashTableDel(g_stHashTable, 1, &stMsg);

    /* 显示 */
    HashTableTraversal(g_stHashTable, 1);
    
#endif

#if BTREE_TESE
    BTREE_MSG_S stMsg;
    BTREE_NODE_S *pstNode1 = NULL;
    BTREE_NODE_S *pstNode2 = NULL;
    BTREE_NODE_S *pstNode3 = NULL;
    BTREE_NODE_S *pstNode4 = NULL;
    BTREE_NODE_S *pstNode5 = NULL;
    BTREE_NODE_S *pstNode6 = NULL;
    BTREE_NODE_S *pstNode7 = NULL;
    BTREE_NODE_S *pstNode8 = NULL;
    BTREE_NODE_S stInsertFlag;
    UINT32 uiValue = 0;
    BTREE_NODE_S *pstFound = NULL;

    stMsg.pvMsg = (UINT32 *)malloc(sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);

    /* 创建根节点 */
    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 0;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    BTreeCreate(&g_stBTree, &stMsg);

    /* 添加节点 */
    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 1;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode1 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, g_stBTree.pstRoot, &stInsertFlag, CBL_NULL, pstNode1);
    
    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 2;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode2 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, g_stBTree.pstRoot, CBL_NULL, &stInsertFlag, pstNode2);

    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 3;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode3 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, pstNode1, &stInsertFlag, CBL_NULL, pstNode3);

    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 4;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode4 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, pstNode1, CBL_NULL, &stInsertFlag, pstNode4);

    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 5;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode5 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, pstNode2, &stInsertFlag, CBL_NULL, pstNode5);

    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 6;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode6 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, pstNode2, CBL_NULL, &stInsertFlag, pstNode6);

    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 7;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode7 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, pstNode5, &stInsertFlag, CBL_NULL, pstNode7);

    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 8;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstNode8 = BTreeMakeNodeWithValue(&stMsg);
    BTreeNodeInsert(&g_stBTree, pstNode5, CBL_NULL, &stInsertFlag, pstNode8);

    /* 前序遍历二叉树 */
    printf("===================Preamble=======================\r\n");
    BTreePreambleTraversal(&g_stBTree, g_stBTree.pstRoot);
    printf("\r\n==================================================\r\n");

    /* 中序遍历二叉树 */
    printf("===================Inorder=======================\r\n");
    BTreeInorderTraversal(&g_stBTree, g_stBTree.pstRoot);
    printf("\r\n==================================================\r\n");

    /* 后序遍历二叉树 */
    printf("===================Postorder=======================\r\n");
    BTreePostorderTraversal(&g_stBTree, g_stBTree.pstRoot);
    printf("\r\n==================================================\r\n");

    printf("Root Address %p\r\n", g_stBTree.pstRoot);

    /* 查找节点1 */
    memset(stMsg.pvMsg, 0, sizeof(UINT32));
    uiValue = 1;
    memcpy(stMsg.pvMsg, &uiValue, sizeof(UINT32));
    stMsg.uiMsgLen = sizeof(UINT32);
    pstFound = (BTREE_NODE_S *)malloc(sizeof(BTREE_NODE_S));
    BTreeLookupNode(&g_stBTree, g_stBTree.pstRoot, &pstFound, &stMsg);

    printf("Main pstFound Address: %p\r\n", pstFound);

    /* 查找确认 */
    if (CBL_NULL != pstFound)
    {
        printf("Found Node: %p LChild: %p RChild: %p ...\r\n", 
                pstFound,
                pstFound->pstLchild,
                pstFound->pstRchild);
    }
    else
    {
        return 0;
    }

    /* 打印去查找到的节点的左子节点值 */
    uiValue = 0;
    printf("Len=%d\r\n", pstFound->pstLchild->pstPayloadMsg->uiMsgLen);
    memcpy(&uiValue, pstFound->pstLchild->pstPayloadMsg->pvMsg, pstFound->pstLchild->pstPayloadMsg->uiMsgLen);
    printf("LChild Node Value = %d\r\n", uiValue);

    /* 销毁二叉树 */
    BTreeDestroy(&g_stBTree);

    /* 确认一下根是否为空 */
    if (CBL_NULL == g_stBTree.pstRoot)
    {
        printf("Release Succ!\r\n");
    }
    else
    {
        printf("Release Fail!\r\n");
    }

#endif

    return 0;
}


