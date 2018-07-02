/*******************************************************************************
*   文  件 名：List.h
*   功     能：链表对外接口
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_BASE_LIST_H_
#define  _CBL_BASE_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/* 根据结构体成员得到该成员在结构体中的偏移量 */
#define OFFSET(TYPE, MEMBER) ((size_t) &(((TYPE*)0)->MEMBER))

/* 根据结构体成员在结构体中的偏移量得到结构体的首地址 */
#define CONTAINER_IN(ptr, TYPE, MEMBER)     \
            (TYPE*)((UINT8*)(ptr) - OFFSET(TYPE, MEMBER))

/* 定义链表头结构 */
typedef struct List_Head_Tag
{
    struct List_Head_Tag *pstNext;
    struct List_Head_Tag *pstPrev;
} LIST_HEAD_S;

/* 定义链表遍历回调处理函数 */
typedef UINT32 (*ForEachHandler)(LIST_HEAD_S *pstList);

BOOL ListHeadInit(LIST_HEAD_S *pstListHead);
BOOL ListIsEmpty(LIST_HEAD_S *pstListHead);
VOID ListAddHead(LIST_HEAD_S *pstListNew, LIST_HEAD_S *pstListHead);
VOID ListAddTail(LIST_HEAD_S *pstListNew, LIST_HEAD_S *pstListHead);
VOID ListDel(LIST_HEAD_S *pstListDel);
VOID ListForEach(LIST_HEAD_S *pstListHead, ForEachHandler fHandler);

#ifdef __cplusplus
}
#endif

#endif

