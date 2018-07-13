/*******************************************************************************
*   文  件 名：Btree.h
*   功     能：二叉树对外接口
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：2018-7-12
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_BASE_BTREE_H_
#define  _CBL_BASE_BTREE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/* 定义外部入二叉树消息结构 */
typedef struct Btree_Msg_Tag
{
    VOID *pvMsg;                    /* 消息 */
    UINT32 uiMsgLen;                /* 消息长度 */
} BTREE_MSG_S;

/* 定义二叉树节点结构 */
typedef struct Btree_Node_Tag
{
    struct Btree_Node_Tag *pstParent;       /* 双亲节点指针 */
    struct Btree_Node_Tag *pstLchild;       /* 左孩子节点指针 */
    struct Btree_Node_Tag *pstRchild;       /* 右孩子节点指针 */
    BTREE_MSG_S *pstPayloadMsg;             /* 节点承载消息内容 */
} BTREE_NODE_S;

/* 定义树结构 */
typedef struct Btree_Tag
{
    BTREE_NODE_S *pstRoot;                  /* 跟节点指针 */
} BTREE_S;

/* 对外接口 */
/* 设置节点承载值 */
VOID BTreeNodeValueSet(BTREE_NODE_S *pstBTreeNode, BTREE_MSG_S *pstMsg);

/* 释放节点内存 */
VOID BTreeNodeRelease(BTREE_NODE_S *pstBTreeNode);

/* 创建二叉树节点 */
BTREE_NODE_S *BTreeMakeNode();

/* 带消息创建二叉树节点 */
BTREE_NODE_S *BTreeMakeNodeWithValue(BTREE_MSG_S *pstMsg);

/* 前序遍历二叉树 */
VOID BTreePreambleTraversal(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode);

/* 中序遍历二叉树 */
VOID BTreeInorderTraversal(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode);

/* 后序遍历二叉树 */
VOID BTreePostorderTraversal(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode);

/* 创建二叉树 */
BOOL BTreeCreate(BTREE_S *pstBtree, BTREE_MSG_S *pstMsg);

/* 向二叉树中插入节点 */
BOOL BTreeNodeInsert(BTREE_S *pstBtree, 
                     BTREE_NODE_S *pstParent, 
                     BTREE_NODE_S *pstLChild, 
                     BTREE_NODE_S *pstRChild,
                     BTREE_NODE_S *pstInsertNode);

/* 销毁二叉树 */
VOID BTreeDestroy(BTREE_S *pstBtree);

/* 根据节点值查找二叉树节点 */
VOID BTreeLookupNode(BTREE_S *pstBtree, BTREE_NODE_S *pstBtreeNode, 
                     BTREE_NODE_S **ppstBtreeFoundNode, BTREE_MSG_S *pstMsg);

#ifdef __cplusplus
}
#endif

#endif

