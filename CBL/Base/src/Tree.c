/*******************************************************************************
*   文  件 名：Tree.c
*   功     能：实现树功能
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#include "List.h"
#include "Tree.h"

/*******************************************************************************
*   函 数   名：TreeNodeValueSet
*   功     能：填充节点负载消息
*   输入参数：pstBTreeNode:树节点
*             pstMsg:负载消息
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
VOID TreeNodeValueSet(TREE_NODE_S *pstTreeNode, TREE_MSG_S *pstMsg)
{
	UINT32 uiChildIndex = 0;
	
    /* 参数校验 */
    if (CBL_NULL == pstTreeNode || CBL_NULL == pstMsg)
    {
        return;
    }

    /* 检查节点负载是否为空，为空则为其申请内存 */
    if (CBL_NULL == pstTreeNode->pstPayloadMsg)
    {
        /* 申请内存 */
        pstTreeNode->pstPayloadMsg = (TREE_MSG_S *)malloc(sizeof(TREE_MSG_S));
        if (CBL_NULL == pstTreeNode->pstPayloadMsg)
        {
            return;
        }

        /* 清空消息内存 */
        memset(pstTreeNode->pstPayloadMsg, 0, sizeof(TREE_MSG_S));
    }

    /* 检查消息是否为空，为空则申请内存 */
    if (CBL_NULL == pstTreeNode->pstPayloadMsg->pvMsg)
    {
        /* 传入消息有误 */
        if (pstMsg->uiMsgLen <= 0)
        {
            return;
        }

        /* 申请内存 */
        pstTreeNode->pstPayloadMsg->pvMsg = (VOID *)malloc(pstMsg->uiMsgLen);
        if (CBL_NULL == pstTreeNode->pstPayloadMsg->pvMsg)
        {
            return;
        }

        /* 清空内存 */
        memset(pstTreeNode->pstPayloadMsg->pvMsg, 0, pstMsg->uiMsgLen);
    }

    /* 填充消息 */
    memcpy(pstTreeNode->pstPayloadMsg->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen);
    pstTreeNode->pstPayloadMsg->uiMsgLen = pstMsg->uiMsgLen;

    /* 清空孩子节点 */
    for (uiChildIndex = 0; uiChildIndex < CBL_TREE_CHILD_MAXNUM; uiChildIndex++)
    {
		pstTreeNode->pastChild[uiChildIndex] = CBL_NULL;
    }

    pstTreeNode->pstParent = CBL_NULL;
    pstTreeNode->pstLSibling = CBL_NULL;
    pstTreeNode->pstRSibling = CBL_NULL;

    return;
}

/*******************************************************************************
*   函 数   名：TreeMakeNodeWithValue
*   功     能：带消息创建树节点
*   输入参数：无
*   输出参数：无
*   返 回 值：CBL_NULL:创建树节点失败
*             BTREE_NODE_S *:创建树节点成功，返回节点指针
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
TREE_NODE_S *TreeMakeNodeWithValue(TREE_MSG_S *pstMsg)
{
    TREE_NODE_S *pstTreeNode = CBL_NULL;
    
    /* 节点内存分配 */
    pstTreeNode = (TREE_NODE_S *)malloc(sizeof(TREE_NODE_S));
    if (CBL_NULL == pstTreeNode)
    {
        return CBL_NULL;
    }

    /* 内存清空 */
    memset(pstTreeNode, 0, sizeof(TREE_NODE_S));

    /* 如果节点值不为空，则要填充消息 */
    if ((CBL_NULL != pstMsg) && (CBL_NULL != pstMsg->pvMsg) && (pstMsg->uiMsgLen > 0))
    {
        TreeNodeValueSet(pstTreeNode, pstMsg);
    }

    return pstTreeNode;
}

/*******************************************************************************
*   函 数   名：TreeLeafNodeRelease
*   功     能：释放叶子节点负载消息
*   输入参数：pstTreeNode:树的叶子节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
static VOID TreeLeafNodeRelease(TREE_NODE_S *pstTreeNode)
{
    /* 参数校验 */
    if (CBL_NULL == pstTreeNode)
    {
        return;
    }

    /* 内存释放 */
    if (CBL_NULL != pstTreeNode->pstPayloadMsg)
    {
        if (CBL_NULL != pstTreeNode->pstPayloadMsg->pvMsg)
        {
            free(pstTreeNode->pstPayloadMsg->pvMsg);
            pstTreeNode->pstPayloadMsg->pvMsg = CBL_NULL;
        }

        free(pstTreeNode->pstPayloadMsg);
        pstTreeNode->pstPayloadMsg = CBL_NULL;
    }

	/* 暂时没用上 */
#if 0
	/* 修改节点的左右兄弟关系 */
	pstTreeNode->pstRSibling->pstLSibling = pstTreeNode->pstLSibling;
	pstTreeNode->pstLSibling->pstRSibling = pstTreeNode->pstRSibling;
#endif

	/* 叶子节点没有孩子节点，不需要删除 */
    /* 释放叶子节点内存 */
    free(pstTreeNode);
    pstTreeNode = CBL_NULL;
    
    return;
}

/*******************************************************************************
*   函 数   名：TreeIsLeafNode
*   功     能：是否是叶子节点
*   输入参数：pstTreeNode:树的节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
BOOL TreeIsLeafNode(TREE_NODE_S *pstTreeNode)
{
	UINT32 uiChildIndex = 0;
	
	/* 参数校验 */
    if (CBL_NULL == pstTreeNode)
    {
        return CBL_FALSE;
    }

	/* 遍历节点的孩子数组，其中一项不为空则说明不是叶子节点 */
	for (uiChildIndex = 0; uiChildIndex < CBL_TREE_CHILD_MAXNUM; uiChildIndex++)
	{
		if (CBL_NULL != pstTreeNode->pastChild[uiChildIndex])
		{
			return CBL_FALSE;
		}
	}

	return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：TreeCreate
*   功     能：创建树
*   输入参数：psttree:树
*             pstMsg:消息
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
BOOL TreeCreate(TREE_S *pstTree, TREE_MSG_S *pstMsg)
{
    TREE_NODE_S *pstTreeNode = CBL_NULL;
    
    /* 参数校验 */
    if ((CBL_NULL == pstTree) || 
        (CBL_NULL == pstMsg) || 
        (CBL_NULL == pstMsg->pvMsg) || 
        (pstMsg->uiMsgLen <= 0))
    {
        return CBL_FALSE;
    }

    /* 创建树的根节点 */
    pstTreeNode = TreeMakeNodeWithValue(pstMsg);
    if (CBL_NULL == pstTreeNode)
    {
        return CBL_FALSE;
    }

    /* 设置节点关系 */
	pstTreeNode->pstParent = CBL_NULL;
	pstTreeNode->pstLSibling = CBL_NULL;
	pstTreeNode->pstRSibling = CBL_NULL;
	memset(pstTreeNode->pastChild, 0, sizeof(struct Tree_Node_Tag *) * CBL_TREE_CHILD_MAXNUM);

	/* 加入根节点 */
	pstTree->pstRoot = pstTreeNode;
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：TreeNodeView
*   功     能：显示节点值
*   输入参数：pstTreeNode:树节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
static VOID TreeNodeView(TREE_NODE_S *pstTreeNode)
{
	UINT32 uiNodeValue = 0;
	
	/* 参数校验 */
	if ((CBL_NULL == pstTreeNode) || 
		(CBL_NULL == pstTreeNode->pstPayloadMsg) ||
		(CBL_NULL == pstTreeNode->pstPayloadMsg->pvMsg) ||
		(pstTreeNode->pstPayloadMsg->uiMsgLen <= 0))
	{
		return;
	}

	/* 暂时以数字作为测试条件 */
	memcpy(&uiNodeValue, pstTreeNode->pstPayloadMsg->pvMsg, sizeof(UINT32));
	printf("%d ", uiNodeValue);

	return;
}

/*******************************************************************************
*   函 数   名：TreeSubNodeTraversal
*   功     能：遍历树
*   输入参数：pstTree:树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
static VOID TreeSubNodeTraversal(TREE_NODE_S *pstTreeNode)
{
	UINT32 uiChildIndex = 0;
	
	/* 参数校验 */
	if (CBL_NULL == pstTreeNode)
	{
		return;
	}

	/* 轮询节点的孩子节点 */
	for (uiChildIndex = 0; uiChildIndex < CBL_TREE_CHILD_MAXNUM; uiChildIndex++)
	{
		if (CBL_NULL == pstTreeNode->pastChild[uiChildIndex])
		{
			continue;
		}

		/* 显示节点值 */
		TreeNodeView(pstTreeNode->pastChild[uiChildIndex]);
		
		/* 继续轮询子节点 */
		TreeSubNodeTraversal(pstTreeNode->pastChild[uiChildIndex]);
	}
	
	return;
}

/*******************************************************************************
*   函 数   名：TreeTraversal
*   功     能：遍历树
*   输入参数：pstTree:树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
VOID TreeTraversal(TREE_S *pstTree)
{
	/* 参数校验 */
	if (CBL_NULL == pstTree || CBL_NULL == pstTree->pstRoot)
	{
		return;
	}

	/* 显示每个节点下面的子节点 */
	/* 显示当前节点 */
	TreeNodeView(pstTree->pstRoot);
	
	/* 轮询根节点 */
	TreeSubNodeTraversal(pstTree->pstRoot);
	
	return;
}

/*******************************************************************************
*   函 数   名：TreeNodeInsert
*   功     能：向树中插入节点
*   输入参数：pstTree:树
*			  pstParentNode:插入节点的父节点
*			  pstIsertNode:插入节点，插入节点的孩子节点必须为空，从父节点的最后一个
*						   可用孩子节点处开始插入
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
VOID TreeNodeInsert(TREE_S *pstTree, TREE_NODE_S *pstParentNode, TREE_NODE_S *pstIsertNode)
{
	UINT32 uiChildIndex = 0;
	
	
	/* 参数校验 */
	if (CBL_NULL == pstTree || CBL_NULL == pstTree->pstRoot)
	{
		return;
	}

	if (CBL_NULL == pstParentNode || CBL_NULL == pstIsertNode)
	{
		return;
	}

	/* 找到待插入节点的子节点没有可用的位置 */
	for (uiChildIndex = 0; uiChildIndex < CBL_TREE_CHILD_MAXNUM; uiChildIndex++)
	{
		if (CBL_NULL != pstParentNode->pastChild[uiChildIndex])
		{
			continue;
		}

		/* 插入 */
		pstParentNode->pastChild[uiChildIndex] = pstIsertNode;
		break;
	}
	
	return;
}

/*******************************************************************************
*   函 数   名：TreeListMakeNode
*   功     能：创建树的链表节点
*   输入参数：pstTree:树
*			  pstTreeListHead:树链表
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
TREE_LIST_NODE_S *TreeListMakeNode(TREE_NODE_S *pstTreeNode)
{
	TREE_LIST_NODE_S *pstTreeListNode = CBL_NULL;

	pstTreeListNode = (TREE_LIST_NODE_S *)malloc(sizeof(TREE_LIST_NODE_S));
	if (CBL_NULL == pstTreeListNode)
	{
		return CBL_NULL;
	}

	memset(pstTreeListNode, 0, sizeof(TREE_LIST_NODE_S));
	pstTreeListNode->pstTreeNode = pstTreeNode;

	return pstTreeListNode;
}

/*******************************************************************************
*   函 数   名：TreeTraversalSubToList
*   功     能：遍历树的子节点并添加到链表
*   输入参数：pstTree:树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
static VOID TreeTraversalSubToList(TREE_NODE_S *pstTreeNode, LIST_HEAD_S *pstTreeListHead)
{
	UINT32 uiChildIndex = 0;
	TREE_LIST_NODE_S *pstTreeListNode = CBL_NULL;
	
	/* 参数校验 */
	if (CBL_NULL == pstTreeNode)
	{
		return;
	}

	/* 轮询节点的孩子节点 */
	for (uiChildIndex = 0; uiChildIndex < CBL_TREE_CHILD_MAXNUM; uiChildIndex++)
	{
		if (CBL_NULL == pstTreeNode->pastChild[uiChildIndex])
		{
			continue;
		}

		/* 创建链表节点 */
		pstTreeListNode = TreeListMakeNode(pstTreeNode->pastChild[uiChildIndex]);
		if (CBL_NULL == pstTreeListNode)
		{
			return;
		}

		/* 将节点加入链表末尾 */
		ListAddHead(&pstTreeListNode->stTreeList, pstTreeListHead);
		
		/* 继续轮询子节点 */
		TreeTraversalSubToList(pstTreeNode->pastChild[uiChildIndex], pstTreeListHead);
	}
	
	return;
}

/*******************************************************************************
*   函 数   名：TreeTraversalToList
*   功     能：遍历树并将节点加入到链表中
*   输入参数：pstTree:树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
VOID TreeTraversalToList(TREE_S *pstTree, LIST_HEAD_S *pstTreeListHead)
{
	TREE_LIST_NODE_S *pstTreeListNode = CBL_NULL;
	
	/* 参数校验 */
	if (CBL_NULL == pstTree || CBL_NULL == pstTree->pstRoot || CBL_NULL == pstTreeListHead)
	{
		return;
	}

	/* 创建链表节点 */
	pstTreeListNode = TreeListMakeNode(pstTree->pstRoot);
	if (CBL_NULL == pstTreeListNode)
	{
		return;
	}

	/* 将节点加入链表末尾 */
	ListAddHead(&pstTreeListNode->stTreeList, pstTreeListHead);
	
	/* 轮询根节点 */
	TreeTraversalSubToList(pstTree->pstRoot, pstTreeListHead);
	
	return;
}

/*******************************************************************************
*   函 数   名：TreeDestroy
*   功     能：销毁树，首先将节点链接成一个链表，然后按链表删除
*   输入参数：pstBtree: 树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
UINT32 TreeListNodeShow(LIST_HEAD_S *pstList)
{
	TREE_LIST_NODE_S *pstListNode = CBL_NULL;
	UINT32 uiTreeNodeValue = 0;

	//printf("Show list entry.\r\n");

	pstListNode = CONTAINER_IN(pstList, TREE_LIST_NODE_S, stTreeList);

	/* 得到节点值 */
	memcpy(&uiTreeNodeValue, pstListNode->pstTreeNode->pstPayloadMsg->pvMsg, pstListNode->pstTreeNode->pstPayloadMsg->uiMsgLen);

	//printf("NodeValue=%d\r\n", uiTreeNodeValue);

	return 0;
}

/*******************************************************************************
*   函 数   名：TreeDestroy
*   功     能：销毁树，首先将节点链接成一个链表，然后按链表删除
*   输入参数：pstBtree: 树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
UINT32 TreeListNodeDelete(LIST_HEAD_S *pstList)
{
	TREE_LIST_NODE_S *pstListNode = CBL_NULL;
	UINT32 uiTreeNodeValue = 0;

	pstListNode = CONTAINER_IN(pstList, TREE_LIST_NODE_S, stTreeList);

	/* 得到节点值 */
	memcpy(&uiTreeNodeValue, pstListNode->pstTreeNode->pstPayloadMsg->pvMsg, pstListNode->pstTreeNode->pstPayloadMsg->uiMsgLen);

	printf("Del NodeValue=%d\r\n", uiTreeNodeValue);

	/* 链表节点删除 */
	ListDel(&pstListNode->stTreeList);
	
	/* 释放节点 */
	TreeLeafNodeRelease(pstListNode->pstTreeNode);

	return 0;
}

/*******************************************************************************
*   函 数   名：TreeDestroy
*   功     能：销毁树，首先将节点链接成一个链表，然后按链表删除
*   输入参数：pstBtree: 树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
VOID TreeDestroy(TREE_S *pstTree)
{
	LIST_HEAD_S stTreeList;

	/* 参数校验 */
	if (CBL_NULL == pstTree || CBL_NULL == pstTree->pstRoot)
	{
		return;
	}

	/* 初始化链表 */
	ListHeadInit(&stTreeList);

	/* 遍历树构成链表 */
	TreeTraversalToList(pstTree, &stTreeList);

	/* 遍历链表 */
	ListForEach(&stTreeList, TreeListNodeShow);

	/* 根据链表里存放的树节点地址删除树节点 */
	ListForEach(&stTreeList, TreeListNodeDelete);

	/* 根节点清空 */
	pstTree->pstRoot = CBL_NULL;

	return;
}

/*******************************************************************************
*   函 数   名：TreeNodeCmp
*   功     能：比较节点值
*   输入参数：pstTreeNode:树节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-14
*   修改历史：无
*******************************************************************************/
static BOOL TreeNodeCmp(TREE_NODE_S *pstTreeNode, TREE_MSG_S *pstMsg)
{
	/* 参数校验 */
	if ((CBL_NULL == pstTreeNode) || 
		(CBL_NULL == pstTreeNode->pstPayloadMsg) ||
		(CBL_NULL == pstTreeNode->pstPayloadMsg->pvMsg) ||
		(pstTreeNode->pstPayloadMsg->uiMsgLen <= 0))
	{
		return CBL_FALSE;
	}

	/* 检查消息有效性 */
	if ((CBL_NULL == pstMsg) ||
		(CBL_NULL == pstMsg->pvMsg) ||
		(pstMsg->uiMsgLen <= 0))
	{
		return CBL_FALSE;
	}

	/* 比较节点 */
	if (pstTreeNode->pstPayloadMsg->uiMsgLen != pstMsg->uiMsgLen)
	{
		return CBL_FALSE;
	}

	/* 两者长度一致，内容一致 */
	if (0 == memcmp(pstTreeNode->pstPayloadMsg->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen))
	{
		return CBL_TRUE;
	}

	return CBL_FALSE;
}

/*******************************************************************************
*   函 数   名：TreeNodeTraversalLookup
*   功     能：遍历查找树节点
*   输入参数：pstTree:树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
static VOID TreeNodeTraversalLookup(TREE_NODE_S *pstTreeNode, TREE_NODE_S **ppstTreeNode, TREE_MSG_S *pstMsg, BOOL *bFoundNode)
{
	UINT32 uiChildIndex = 0;
	BOOL bRet = CBL_FALSE;
	
	/* 参数校验 */
	if (CBL_NULL == pstTreeNode)
	{
		return;
	}

	/* 轮询节点的孩子节点 */
	for (uiChildIndex = 0; uiChildIndex < CBL_TREE_CHILD_MAXNUM; uiChildIndex++)
	{
		if (CBL_NULL == pstTreeNode->pastChild[uiChildIndex])
		{
			continue;
		}

		if (!(*bFoundNode))
		{
			/* 显示节点值 */
			bRet = TreeNodeCmp(pstTreeNode->pastChild[uiChildIndex], pstMsg);
			if (bRet)
			{
				/* 找到了 */
	            *ppstTreeNode = pstTreeNode->pastChild[uiChildIndex];
	            *bFoundNode = CBL_TRUE;
	            return;
			}

			/* 继续轮询子节点 */
			TreeNodeTraversalLookup(pstTreeNode->pastChild[uiChildIndex], ppstTreeNode, pstMsg, bFoundNode);
		}
	}
	
	return;
}

/*******************************************************************************
*   函 数   名：TreeNodeLookup
*   功     能：查找节点
*   输入参数：pstTree:树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/
BOOL TreeNodeLookup(TREE_S *pstTree, TREE_NODE_S **ppstTreeNode, TREE_MSG_S *pstMsg)
{
	BOOL bRet = CBL_FALSE;
	
	/* 解决快速退出递归问题 */
    static BOOL bFoundNode = CBL_FALSE;
    
	/* 参数校验 */
	if (CBL_NULL == pstTree || CBL_NULL == pstTree->pstRoot)
	{
		return CBL_FALSE;
	}

	bRet = TreeNodeCmp(pstTree->pstRoot, pstMsg);
	if (bRet)
    {
        /* 找到了 */
        *ppstTreeNode = pstTree->pstRoot;
        bFoundNode = CBL_TRUE;
        return bFoundNode;
    }
	
	/* 轮询根节点 */
	TreeNodeTraversalLookup(pstTree->pstRoot, ppstTreeNode, pstMsg, &bFoundNode);
	
	return bFoundNode;
}


