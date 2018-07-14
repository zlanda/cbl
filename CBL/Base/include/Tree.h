/*******************************************************************************
*   文  件 名：Tree.h
*   功     能：树对外接口
*   作     者：zhanxc
*   E-Mail  : zhanxc_chpu@sina.com
*   创建日期：2018-7-13
*   修改历史：无
*******************************************************************************/

#ifndef  _CBL_BASE_TREE_H_
#define  _CBL_BASE_TREE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 内部头文件 */
#include "Types.h"

/* 定义孩子节点个数 */
/* 暂时定义一个节点最多有10个孩子节点 */
#define CBL_TREE_CHILD_MAXNUM           10

/* 定义树承载的数据结构 */
typedef struct Tree_Msg_Tag
{
    VOID *pvMsg;            /* 承载消息 */
    UINT32 uiMsgLen;        /* 承载消息长度 */
} TREE_MSG_S;

/* 定义树节点结构 */
typedef struct Tree_Node_Tag
{
    struct Tree_Node_Tag *pstParent;                        /* 节点的父节点 */
    struct Tree_Node_Tag *pstLSibling;                      /* 节点的左兄弟节点 */
    struct Tree_Node_Tag *pstRSibling;                      /* 节点的右兄弟节点 */
    struct Tree_Node_Tag *pastChild[CBL_TREE_CHILD_MAXNUM];   /* 节点的孩子节点 */
    TREE_MSG_S *pstPayloadMsg;                              /* 节点承载消息内容 */
} TREE_NODE_S;

/* 定义树结构 */
typedef struct Tree_Tag
{
    TREE_NODE_S *pstRoot;             /* 指向树的根节点 */
} TREE_S;

/* 定义树链表结构 */
typedef struct Tree_List_Tag
{
	TREE_NODE_S *pstTreeNode;			/* 树的节点 */
	LIST_HEAD_S stTreeList;				/* 树链表 */
} TREE_LIST_NODE_S;

/* 定义树的外部接口 */
VOID TreeNodeValueSet(TREE_NODE_S *pstTreeNode, TREE_MSG_S *pstMsg);

TREE_NODE_S *TreeMakeNodeWithValue(TREE_MSG_S *pstMsg);

BOOL TreeIsLeafNode(TREE_NODE_S *pstTreeNode);

/* 销毁树 */
VOID TreeDestroy(TREE_S *pstTree);

/* 创建树 */
BOOL TreeCreate(TREE_S *pstTree, TREE_MSG_S *pstMsg);

/* 遍历树 */
VOID TreeTraversal(TREE_S *pstTree);

/* 向树种插入节点 */
VOID TreeNodeInsert(TREE_S *pstTree, TREE_NODE_S *pstParentNode, TREE_NODE_S *pstIsertNode);

/* 查找结点 */
BOOL TreeNodeLookup(TREE_S *pstTree, TREE_NODE_S **ppstTreeNode, TREE_MSG_S *pstMsg);

#ifdef __cplusplus
}
#endif

#endif

