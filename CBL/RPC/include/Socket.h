/*******************************************************************************
*   ��  �� ����Socket.h
*   ��     �ܣ�Unix Socket IPC����ӿ�
*   ��     �ߣ�zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/

#ifndef  _CBL_RPC_SOCKET_H_
#define  _CBL_RPC_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ϵͳͷ�ļ� */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <errno.h>
#include <sys/un.h>

/* �ڲ�ͷ�ļ� */
#include "Types.h"

/* ��������� */
/* ����·������ */
#define CBL_CREATE_UNIX_SERVER_SOCKET_PATH_VALID		-1

/* ����socket����ʧ�� */
#define CBL_CREATE_UNIX_SERVER_SOCKET_CREATE_FAILED		-2

/* ����socket������ģʽʧ�� */
#define CBL_CREATE_UNIX_SERVER_SOCKET_MODE_FAILED		-3

/* ����socket��ʧ�� */
#define CBL_CREATE_UNIX_SERVER_SOCKET_BIND_FAILED		-4

/* ����socket����ʧ�� */
#define CBL_CREATE_UNIX_SERVER_SOCKET_LISTEN_FAILED		-5

/* ����·������ */
#define CBL_CREATE_UNIX_CLIENT_SOCKET_PATH_VALID		-1

/* ����socket����ʧ�� */
#define CBL_CREATE_UNIX_CLIENT_SOCKET_CREATE_FAILED		-2

/* ����socket����ʧ�� */
#define CBL_CREATE_UNIX_CLIENT_SOCKET_CONNECT_FAILED	-3

/* �ȴ��ͻ�������ʧ�� */
#define CBL_ACCEPT_FAILED								-1

/* ���巢����Ϣʱsocket�ļ��������Ƿ� */
#define CBL_SEND_MESSAGE_MSG_SOCKET_VALID				-11111111

/* ���巢����Ϣʱ�����Ƿ� */
#define CBL_SEND_MESSAGE_MSG_PARAM_VALID				-22222222

/* ����socket����ӿ� */
/* ����socket fdΪ������ģʽ */
BOOL SocketNonBlockSet(INT32 iSockFd);

/* ������������socket */
INT32 CreateUnixServerTCPSocket(CHAR *pcSockPath, UINT32 uiMaxConnNum);

/* �����ͻ���socket */
INT32 CreateUnixClientTCPSocket(CHAR *pcSockPath);

/* �ȴ��ͻ������� */
INT32 AcceptUnixSocket(INT32 iSockFd);

/* �ر�socket */
VOID CloseUnixSocket(INT32 iSockFd);

/* ������Ϣ */
INT32 SendMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags);

/* ������Ϣ */
INT32 RecvMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags);

/* ������������socket */
INT32 CreateUnixServerUDPSocket(CHAR *pcSockPath);

/* �����ͻ���socket */
INT32 CreateUnixClientUDPSocket(CHAR *pcSockPath);

#ifdef __cplusplus
}
#endif

#endif

