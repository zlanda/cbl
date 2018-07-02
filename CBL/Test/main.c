#include <stdio.h>
#include <stdlib.h>
#include "List.h"

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


int main()
{
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
    
    return 0;
}


