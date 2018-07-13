/*******************************************************************************
*   文  件 名：Btree.c
*   功     能：实现二叉树功能
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

#include "Btree.h"

/*******************************************************************************
*   函 数   名：BTreeNodeValueSet
*   功     能：填充节点负载消息
*   输入参数：pstBTreeNode:二叉树节点
*             pstMsg:负载消息
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreeNodeValueSet(BTREE_NODE_S *pstBTreeNode, BTREE_MSG_S *pstMsg)
{
    /* 参数校验 */
    if (CBL_NULL == pstBTreeNode || CBL_NULL == pstMsg)
    {
        return;
    }

    /* 检查节点负载是否为空，为空则为其申请内存 */
    if (CBL_NULL == pstBTreeNode->pstPayloadMsg)
    {
        /* 申请内存 */
        pstBTreeNode->pstPayloadMsg = (BTREE_MSG_S *)malloc(sizeof(BTREE_MSG_S));
        if (CBL_NULL == pstBTreeNode->pstPayloadMsg)
        {
            return;
        }

        /* 清空消息内存 */
        memset(pstBTreeNode->pstPayloadMsg, 0, sizeof(BTREE_MSG_S));
    }

    /* 检查消息是否为空，为空则申请内存 */
    if (CBL_NULL == pstBTreeNode->pstPayloadMsg->pvMsg)
    {
        /* 传入消息有误 */
        if (pstMsg->uiMsgLen <= 0)
        {
            return;
        }

        /* 申请内存 */
        pstBTreeNode->pstPayloadMsg->pvMsg = (VOID *)malloc(pstMsg->uiMsgLen);
        if (CBL_NULL == pstBTreeNode->pstPayloadMsg->pvMsg)
        {
            return;
        }

        /* 清空内存 */
        memset(pstBTreeNode->pstPayloadMsg->pvMsg, 0, pstMsg->uiMsgLen);
    }

    /* 填充消息 */
    memcpy(pstBTreeNode->pstPayloadMsg->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen);
    pstBTreeNode->pstPayloadMsg->uiMsgLen = pstMsg->uiMsgLen;

    return;
}

/*******************************************************************************
*   函 数   名：BTreeNodeRelease
*   功     能：释放节点负载消息
*   输入参数：pstBTreeNode:二叉树节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreeNodeRelease(BTREE_NODE_S *pstBTreeNode)
{
    /* 参数校验 */
    if (CBL_NULL == pstBTreeNode)
    {
        return;
    }

    /* 内存释放 */
    if (CBL_NULL != pstBTreeNode->pstPayloadMsg)
    {
        if (CBL_NULL != pstBTreeNode->pstPayloadMsg->pvMsg)
        {
            free(pstBTreeNode->pstPayloadMsg->pvMsg);
            pstBTreeNode->pstPayloadMsg->pvMsg = CBL_NULL;
        }

        free(pstBTreeNode->pstPayloadMsg);
        pstBTreeNode->pstPayloadMsg = CBL_NULL;
    }

    /* 释放节点内存 */
    free(pstBTreeNode);
    
    return;
}

/*******************************************************************************
*   函 数   名：BTreeMakeNode
*   功     能：创建二叉树节点
*   输入参数：无
*   输出参数：无
*   返 回 值：CBL_NULL:创建二叉树节点失败
*             BTREE_NODE_S *:创建二叉树节点成功，返回节点指针
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
BTREE_NODE_S *BTreeMakeNode()
{
    BTREE_NODE_S *pstBTreeNode = CBL_NULL;
    
    /* 节点内存分配 */
    pstBTreeNode = (BTREE_NODE_S *)malloc(sizeof(BTREE_NODE_S));
    if (CBL_NULL == pstBTreeNode)
    {
        return CBL_NULL;
    }

    /* 内存清空 */
    memset(pstBTreeNode, 0, sizeof(BTREE_NODE_S));

    return pstBTreeNode;
}

/*******************************************************************************
*   函 数   名：BTreeMakeNodeWithValue
*   功     能：带消息创建二叉树节点
*   输入参数：无
*   输出参数：无
*   返 回 值：CBL_NULL:创建二叉树节点失败
*             BTREE_NODE_S *:创建二叉树节点成功，返回节点指针
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
BTREE_NODE_S *BTreeMakeNodeWithValue(BTREE_MSG_S *pstMsg)
{
    BTREE_NODE_S *pstBTreeNode = CBL_NULL;
    
    /* 节点内存分配 */
    pstBTreeNode = (BTREE_NODE_S *)malloc(sizeof(BTREE_NODE_S));
    if (CBL_NULL == pstBTreeNode)
    {
        return CBL_NULL;
    }

    /* 内存清空 */
    memset(pstBTreeNode, 0, sizeof(BTREE_NODE_S));

    /* 如果节点值不为空，则要填充消息 */
    if ((CBL_NULL != pstMsg) && (CBL_NULL != pstMsg->pvMsg) && (pstMsg->uiMsgLen > 0))
    {
        BTreeNodeValueSet(pstBTreeNode, pstMsg);
    }

    return pstBTreeNode;
}

/*******************************************************************************
*   函 数   名：BTreeNodeView
*   功     能：显示节点的值
*   输入参数：pstBtreeNode:二叉树节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-02
*   修改历史：无
*******************************************************************************/
static VOID BTreeNodeView(BTREE_NODE_S *pstBtreeNode)
{
    /* 以整数作为测试条件 */
    UINT32 uiPayloadValue = 0;

    /* 参数有效性校验 */
    if ((CBL_NULL == pstBtreeNode) ||
        (CBL_NULL == pstBtreeNode->pstPayloadMsg) ||
        (CBL_NULL == pstBtreeNode->pstPayloadMsg->pvMsg) ||
        (pstBtreeNode->pstPayloadMsg->uiMsgLen <= 0))
    {
        return;
    }

    /* 暂时以数字进行测试 */
    memcpy(&uiPayloadValue, pstBtreeNode->pstPayloadMsg->pvMsg, sizeof(UINT32));
    printf("%d ", uiPayloadValue);

    return;
}

/*******************************************************************************
*   函 数   名：BTreePreambleTraversal
*   功     能：前序遍历二叉树
*   输入参数：pstBtree:二叉树
*             pstBtreeNode:根节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-02
*   修改历史：无
*******************************************************************************/
VOID BTreePreambleTraversal(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode)
{
    /* 参数校验 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot || CBL_NULL == pstBtreeNode)
    {
        return;
    }

    /* 中 */
    BTreeNodeView(pstBtreeNode);

    /* 左 */
    BTreePreambleTraversal(pstBtree, pstBtreeNode->pstLchild);

    /* 右 */
    BTreePreambleTraversal(pstBtree, pstBtreeNode->pstRchild);

    return;
}

/*******************************************************************************
*   函 数   名：BTreeInorderTraversal
*   功     能：中序遍历二叉树
*   输入参数：pstBtree:二叉树
*             pstBtreeNode:根节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreeInorderTraversal(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode)
{
    /* 参数校验 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot || CBL_NULL == pstBtreeNode)
    {
        return;
    }

    /* 左 */
    BTreeInorderTraversal(pstBtree, pstBtreeNode->pstLchild);
    
    /* 中 */
    BTreeNodeView(pstBtreeNode);

    /* 右 */
    BTreeInorderTraversal(pstBtree, pstBtreeNode->pstRchild);

    return;
}

/*******************************************************************************
*   函 数   名：BTreePostorderTraversal
*   功     能：后序遍历二叉树
*   输入参数：pstBtree:二叉树
*             pstBtreeNode:根节点
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreePostorderTraversal(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode)
{
    /* 参数校验 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot || CBL_NULL == pstBtreeNode)
    {
        return;
    }

    /* 左 */
    BTreePostorderTraversal(pstBtree, pstBtreeNode->pstLchild);

    /* 右 */
    BTreePostorderTraversal(pstBtree, pstBtreeNode->pstRchild);

    /* 中 */
    BTreeNodeView(pstBtreeNode);

    return;
}

/*******************************************************************************
*   函 数   名：BTreeCreate
*   功     能：创建二叉树
*   输入参数：pstBtree:二叉树
*             pstMsg:根节点值
*   输出参数：无
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-02
*   修改历史：无
*******************************************************************************/
BOOL BTreeCreate(BTREE_S *pstBtree, BTREE_MSG_S *pstMsg)
{
    BTREE_NODE_S *pstBtreeNode = CBL_NULL;
    
    /* 参数校验 */
    if ((CBL_NULL == pstBtree) || 
        (CBL_NULL == pstMsg) || 
        (CBL_NULL == pstMsg->pvMsg) || 
        (pstMsg->uiMsgLen <= 0))
    {
        return CBL_FALSE;
    }

    /* 先创建节点 */
    pstBtreeNode = BTreeMakeNodeWithValue(pstMsg);
    if (CBL_NULL == pstBtreeNode)
    {
        return CBL_FALSE;
    }

    /* 对于父节点来说，其双亲节点为空 */
    pstBtreeNode->pstParent = CBL_NULL;
    pstBtreeNode->pstLchild = CBL_NULL;
    pstBtreeNode->pstRchild = CBL_NULL;
    pstBtree->pstRoot = pstBtreeNode;
    
    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：BTreeNodeInsert
*   功     能：向二叉树中插入节点，当前插入节点的父节点为pstParent，如果指定了
*             pstLChild，则将当前节点作为左子节点插入；如果指定了pstRChild，则
*             将当前节点作为右子节点插入。
*             注意:如果要插入节点为父节点的左子节点，而该父节点又存在左子节点，
*                  这种情况不应该插入；同理右子节点处理一样
*             节点的创建在调用此函数之前完成,其有效性由调用者保证
*   输入参数：pstBtree:二叉树
*             pstParent:当前插入节点的父节点
*             pstLChild:当前节点以左子节点插入
*             pstRChild:当前节点以右子节点插入
*             pstInsertNode:当前插入节点
*   输出参数：无
*   缺     点：只能手动构建树
*   优     点：构造树比较简单
*   返 回 值：CBL_FALSE:失败
*             CBL_TRUE:成功
*   作     者：zhanxc
*   创建日期：2018-7-02
*   修改历史：无
*******************************************************************************/
BOOL BTreeNodeInsert(BTREE_S *pstBtree, 
                     BTREE_NODE_S *pstParent, 
                     BTREE_NODE_S *pstLChild, 
                     BTREE_NODE_S *pstRChild,
                     BTREE_NODE_S *pstInsertNode)
{
    /* 当前节点不能是根节点，根节点单独创建 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot)
    {
        return CBL_FALSE;
    }

    /* 当前插入节点的父节点不能为空 */
    if (CBL_NULL == pstParent || CBL_NULL == pstInsertNode)
    {
        return CBL_FALSE;
    }

    /* 指定了父节点直接插入就行了，不需要查找 */
    /* 左子节点插入 */
    if (CBL_NULL != pstLChild)
    {
        if (CBL_NULL != pstParent->pstLchild)
        {
            return CBL_FALSE;
        }
        else
        {
            pstParent->pstLchild = pstInsertNode;
        }
    }

    /* 右子节点插入 */
    if (CBL_NULL != pstRChild)
    {
        if (CBL_NULL != pstParent->pstRchild)
        {
            return CBL_FALSE;
        }
        else
        {
            pstParent->pstRchild = pstInsertNode;
        }
    }

    return CBL_TRUE;
}

/*******************************************************************************
*   函 数   名：BTreeDestroy
*   功     能：销毁二叉树
*   输入参数：pstBtree:二叉树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreeNodeFree(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode)
{
    /* 参数校验 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot || CBL_NULL == pstBtreeNode)
    {
        return;
    }

    /* 左 */
    BTreeNodeFree(pstBtree, pstBtreeNode->pstLchild);

    /* 右 */
    BTreeNodeFree(pstBtree, pstBtreeNode->pstRchild);

    /* 中 */
    BTreeNodeRelease(pstBtreeNode);

    return;
}

/*******************************************************************************
*   函 数   名：BTreeDestroy
*   功     能：销毁二叉树
*   输入参数：pstBtree:二叉树
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreeDestroy(BTREE_S *pstBtree)
{
    /* 参数校验 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot)
    {
        return;
    }

    /* 释放节点内存 */
    BTreeNodeFree(pstBtree, pstBtree->pstRoot);

    /* 将根节点置为空 */
    pstBtree->pstRoot = CBL_NULL;
    
    return;
}

/*******************************************************************************
*   函 数   名：BTreeNodeCmp
*   功     能：比较节点值
*   输入参数：pstBtreeNode:二叉树节点
*             pstMsg:匹配消息
*   输出参数：无
*   返 回 值：CBL_FALSE:不匹配
*             CBL_TRUE:匹配
*   作     者：zhanxc
*   创建日期：2018-7-02
*   修改历史：无
*******************************************************************************/
static BOOL BTreeNodeCmp(BTREE_NODE_S *pstBtreeNode, BTREE_MSG_S *pstMsg)
{
    /* 参数有效性校验 */
    if ((CBL_NULL == pstBtreeNode) ||
        (CBL_NULL == pstBtreeNode->pstPayloadMsg) ||
        (CBL_NULL == pstBtreeNode->pstPayloadMsg->pvMsg) ||
        (pstBtreeNode->pstPayloadMsg->uiMsgLen <= 0))
    {
        return CBL_FALSE;
    }

    /* 比较节点值 */
    if (pstBtreeNode->pstPayloadMsg->uiMsgLen != pstMsg->uiMsgLen)
    {
        return CBL_FALSE;
    }
    
    if (0 == memcmp(pstBtreeNode->pstPayloadMsg->pvMsg, pstMsg->pvMsg, pstMsg->uiMsgLen))
    {
        return CBL_TRUE;
    }

    return CBL_FALSE;
}

/*******************************************************************************
*   函 数   名：BTreeLookupNode
*   功     能：根据节点值查找二叉树节点
*   输入参数：pstBtree:二叉树
*             pstBtreeNode:二叉树根节点
*             pstMsg:消息内容
*             ppstBtreeFoundNode:找到的节点指针，外部传入时确保其为NULL
*   输出参数：无
*   返 回 值：返回消息匹配正确的节点
*   作     者：zhanxc
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/
VOID BTreeLookupNode(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode, 
                     BTREE_NODE_S **ppstBtreeFoundNode, BTREE_MSG_S *pstMsg)
{
    BOOL bRet = CBL_FALSE;
    
    /* 解决快速退出递归问题 */
    static BOOL bFoundNode = CBL_FALSE;

    /* 参数校验 */
    if (CBL_NULL == pstBtree || CBL_NULL == pstBtree->pstRoot || CBL_NULL == pstBtreeNode)
    {
        return;
    }

    /* bFoundNode为False则表示没找到还需要继续进行递归查找;
       bFoundNode为True则表示已经找到了，不需要再继续进行查找 */
    if (!bFoundNode)
    {
        /* 中 */
        bRet = BTreeNodeCmp(pstBtreeNode, pstMsg);
        if (bRet)
        {
            /* 找到了 */
            *ppstBtreeFoundNode = pstBtreeNode;
            bFoundNode = CBL_TRUE;
            return;
        }

        /* 左 */
        BTreeLookupNode(pstBtree, pstBtreeNode->pstLchild, ppstBtreeFoundNode, pstMsg);

        /* 右 */
        BTreeLookupNode(pstBtree, pstBtreeNode->pstRchild, ppstBtreeFoundNode, pstMsg);
    }
    
    return;
}


