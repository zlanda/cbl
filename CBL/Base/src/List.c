/*******************************************************************************
*   文  件 名：List.c
*   功     能：实现链表功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#include <List.h>

/*******************************************************************************
*   函 数   名：ListHeadInit
*   功     能：链表初始化函数
*   输入参数：pstListHead:待初始化的链表头
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/
BOOL ListHeadInit(LIST_HEAD_S *pstListHead)
{
    /* 参数校验 */
    if (CBL_NULL == pstListHead)
    {
        return CBL_FALSE;
    }
    
    pstListHead->pstNext = pstListHead;
    pstListHead->pstPrev = pstListHead;
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：ListHeadInit
*   功     能：链表是否为空
*   输入参数：pstListHead:待初始化的链表头
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/
BOOL ListIsEmpty(LIST_HEAD_S *pstListHead)
{
    /* 参数校验 */
    if (CBL_NULL == pstListHead)
    {
        return CBL_FALSE;
    }

    if (pstListHead->pstNext == pstListHead)
    {
        return CBL_TRUE;
    }
    
    return CBL_FALSE;
}

/*******************************************************************************
*   函 数   名：ListAddHead
*   功     能：将链表节点插入到链表头
*   输入参数：pstListHead:待初始化的链表头
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/
VOID ListAddHead(LIST_HEAD_S *pstListNew, LIST_HEAD_S *pstListHead)
{
    /* 参数校验 */
    if ((CBL_NULL == pstListHead) || (CBL_NULL == pstListNew))
    {
        return;
    }

    pstListNew->pstPrev = pstListHead;
    pstListNew->pstNext = pstListHead->pstNext;    
    pstListHead->pstNext->pstPrev = pstListNew;
    pstListHead->pstNext = pstListNew;
    
    return;
}

/*******************************************************************************
*   函 数   名：ListAddTail
*   功     能：将链表节点插入到链表尾
*   输入参数：pstListHead:待初始化的链表头
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/
VOID ListAddTail(LIST_HEAD_S *pstListNew, LIST_HEAD_S *pstListHead)
{
    /* 参数校验 */
    if ((CBL_NULL == pstListHead) || (CBL_NULL == pstListNew))
    {
        return;
    }

    pstListNew->pstPrev = pstListHead->pstPrev;
    pstListNew->pstNext = pstListHead;
    pstListHead->pstPrev->pstNext = pstListNew;
    pstListHead->pstPrev = pstListNew;
    
    return;
}

/*******************************************************************************
*   函 数   名：ListDel
*   功     能：删除链表节点
*   输入参数：pstListDel:待删除链表节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/
VOID ListDel(LIST_HEAD_S *pstListDel)
{
    /* 参数校验 */
    if (CBL_NULL == pstListDel)
    {
        return;
    }

	pstListDel->pstNext->pstPrev = pstListDel->pstPrev;
	pstListDel->pstPrev->pstNext = pstListDel->pstNext;
    
    return;
}

/*******************************************************************************
*   函 数   名：ListForEach
*   功     能：遍历链表节点
*   输入参数：pstListDel:待删除链表节点
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/
VOID ListForEach(LIST_HEAD_S *pstListHead, ForEachHandler fHandler)
{
    BOOL bListIsEmpty = CBL_FALSE;
    LIST_HEAD_S *pstList = CBL_NULL;

    /* 参数校验 */
    if (CBL_NULL == pstListHead)
    {
        return;
    }

    /* 检查链表是否为空，如果为空则不需要处理 */
    bListIsEmpty = ListIsEmpty(pstListHead);
    if (bListIsEmpty)
    {
        return;
    }

    /* 到这里了说明链表肯定不为空，开始遍历 */
    for (pstList = pstListHead->pstNext; pstList != pstListHead; pstList = pstList->pstNext)
    {
        fHandler(pstList);
    }

    return;
}


