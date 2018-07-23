/*******************************************************************************
*   ��  �� ����Socket.c
*   ��     �ܣ���װUnix Socket IPC����
*   ��     �ߣ�zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/

#include "Socket.h"

/*******************************************************************************
*   �� ��   ����SocketNonBlockSet
*   ��     �ܣ�����socket fdΪ������ģʽ
*   ���������iSockFd:socket�ļ�������
*   �����������
*   �� �� ֵ��CBL_FALSE:����ʧ��
*             CBL_TRUE:���óɹ�
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
BOOL SocketNonBlockSet(INT32 iSockFd)  
{  
    INT32 iSockOptions;  

    iSockOptions = fcntl(iSockFd, F_GETFL);  
    if (iSockOptions < 0)  
    {  
		return CBL_FALSE;
    }
	
    iSockOptions = (iSockOptions | O_NONBLOCK);  
    if (fcntl(iSockFd, F_SETFL, iSockOptions) < 0)  
    {  
		return CBL_FALSE; 
    }

	return CBL_TRUE;
}

/*******************************************************************************
*   �� ��   ����CreateUnixServerTCPSocket
*   ��     �ܣ�����Unix Socket��������
*   ���������pcSockPath:PATH�ļ�·��
*             uiMaxConnNum:������Ӹ���
*   �����������
*   �� �� ֵ��>0:socket�ļ�������
*             <0:����ʧ��
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-18
*   �޸���ʷ����
*******************************************************************************/
INT32 CreateUnixServerTCPSocket(CHAR *pcSockPath, UINT32 uiMaxConnNum)
{
	INT32 iSockFd = -1;
#if 0
	BOOL bSockOptions = CBL_FALSE;
#endif
	struct sockaddr_un stSockAddr;

	if (CBL_NULL == pcSockPath)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_PATH_VALID;
	}

	/* ����socket */
	iSockFd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (iSockFd < 0)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_CREATE_FAILED;
	}

#if 0
	/* ����socket�ļ�������Ϊ������ģʽ */
	bSockOptions = SocketNonBlockSet(iSockFd);
	if (!bSockOptions)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_MODE_FAILED;
	}
#endif

	/* ����socket��ַ */
	bzero(&stSockAddr, sizeof(stSockAddr));
	stSockAddr.sun_family = AF_UNIX;
	memcpy(stSockAddr.sun_path, pcSockPath, strlen(pcSockPath) + 1);

	/* socket���ַ�� */
	if (bind(iSockFd, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)) < 0)
	{  
		return CBL_CREATE_UNIX_SERVER_SOCKET_BIND_FAILED;
    }

	/* �����ͻ������� */
	if (listen(iSockFd, uiMaxConnNum) < 0)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_LISTEN_FAILED;
	}

	return iSockFd;
}

/*******************************************************************************
*   �� ��   ����CreateUnixClientTCPSocket
*   ��     �ܣ�����Unix Socket�ͻ���
*   ���������pcSockPath:PATH�ļ�·��
*   �����������
*   �� �� ֵ��>0:socket�ļ�������
*             <0:����ʧ��
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
INT32 CreateUnixClientTCPSocket(CHAR *pcSockPath)
{
	INT32 iSockFd = -1;
	INT32 iOnFlag = 1;
	struct sockaddr_un stServerAddr;

	if (CBL_NULL == pcSockPath)
	{
        return CBL_CREATE_UNIX_CLIENT_SOCKET_PATH_VALID;  
	}

	/* ����socket */
	if((iSockFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {  
        return CBL_CREATE_UNIX_CLIENT_SOCKET_CREATE_FAILED;  
    }

	setsockopt(iSockFd, SOL_SOCKET, SO_REUSEADDR, &iOnFlag, sizeof(iOnFlag));

	/* ���õ�ַ */
	memset(&stServerAddr, 0, sizeof(stServerAddr));
	stServerAddr.sun_family = AF_UNIX;  
	memcpy(stServerAddr.sun_path, pcSockPath, strlen(pcSockPath) + 1);

	/* ���ӷ������� */
    if(connect(iSockFd, (struct sockaddr*)&stServerAddr, sizeof(stServerAddr)) < 0)
    {  
        return CBL_CREATE_UNIX_CLIENT_SOCKET_CONNECT_FAILED;  
    }

	return iSockFd;
}

/*******************************************************************************
*   �� ��   ����AcceptUnixSocket
*   ��     �ܣ��ȴ��ͻ�������
*   ���������iSockFd:socket�ļ�������
*   �����������
*   �� �� ֵ������������Ӷ��з�����һ����������ӣ������������Ӷ���Ϊ�գ�
*			  ��ô���̱�Ͷ��˯��
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
INT32 AcceptUnixSocket(INT32 iSockFd)
{
	struct sockaddr_un stRemoteSockAddr;
	socklen_t iSockAddrLen = 0;
	INT32 iConnectFd = -1;

	/* �ļ��������Ƿ��������Ѿ��ر� */
	if (iSockFd < 0)
	{
		return CBL_ACCEPT_FAILED;
	}
	
	memset(&stRemoteSockAddr, 0, sizeof(stRemoteSockAddr));
	iSockAddrLen = sizeof(stRemoteSockAddr);

	iConnectFd = accept(iSockFd, (struct sockaddr *)&stRemoteSockAddr, &iSockAddrLen);
	if (iConnectFd < 0)
	{
		return CBL_ACCEPT_FAILED;
	}
	
	return iConnectFd;
}

/*******************************************************************************
*   �� ��   ����CloseUnixSocket
*   ��     �ܣ��ر�Unix Socket
*   ���������iSockFd:socket�ļ�������
*   �����������
*   �� �� ֵ����
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
VOID CloseUnixSocket(INT32 iSockFd)
{
	/* �ļ��������Ƿ��������Ѿ��ر� */
	if (iSockFd < 0)
	{
		return;
	}

	/* �ر�socket�ļ������� */
	close(iSockFd);
	
	return;
}

/*******************************************************************************
*   �� ��   ����SendMessage
*   ��     �ܣ�������Ϣ
*   ���������iSockFd:socket�ļ�������
*			  pcBuffer:�����͵���Ϣ����
*			  iBufferLen:�����͵���Ϣ����
*			  iFlags:����ִ�з�ʽ
*   �����������
*   �� �� ֵ�����ͳɹ����ط������ݵ�������ע��������ֿ���С��iBufferLen�����涨�Ĵ�С
*			  ʧ�ܷ��ش�����
*	�� �� ��: WSANOTINITIALISED����ʹ�ô�API֮ǰӦ���ȳɹ��ص���WSAStartup()��
*			  WSAENETDOWN��WINDOWS�׽ӿ�ʵ�ּ�⵽������ϵͳʧЧ��
*			  WSAEACESS��Ҫ���ַΪ�㲥��ַ������ر�־δ����ȷ���á�
*			  WSAEINTR��ͨ��һ��WSACancelBlockingCall()��ȡ��һ���������ģ����á�
*			  WSAEINPROGRESS��һ��������WINDOWS�׽ӿڵ������������С�
*			  WSAEFAULT��buf���������û���ַ�ռ��е���Чλ�á�
*			  WSAENETRESET������WINDOWS�׽ӿ�ʵ�ַ��������ӣ��ʸ����ӱ��豻��λ��
*			  WSAENOBUFS��WINDOWS�׽ӿ�ʵ�ֱ���һ��������������
*			  WSAENOTCONN���׽ӿ�δ�����ӡ�
*			  WSAENOTSOCK�������ֲ���һ���׽ӿڡ�
*			  WSAEOPNOTSUPP����������MSG_OOB�����׽ӿڷ�SOCK_STREAM���͡�
*			  WSAESHUTDOWN���׽ӿ��ѱ��رա�һ���׽ӿ���1��2��how��������shutdown()�رպ��޷�����send()������
*			  WSAEWOULDBLOCK��
*			  WSAEMSGSIZE���׽ӿ�ΪSOCK_DGRAM���ͣ������ݱ�����WINDOWS�׽ӿ�ʵ����֧�ֵ����ֵ��
*			  WSAEINVAL���׽ӿ�δ��bind()����
*			  WSAECONNABORTED�����ڳ�ʱ������ԭ���������·���жϡ�
*			  WSAECONNRESET�����·��Զ�˸�λ��
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
INT32 SendMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags)
{
	INT32 iSendRet = 0;

	/* У����� */
	if (CBL_NULL == pcBuffer || iBufferLen <= 0)
	{
		return CBL_SEND_MESSAGE_MSG_PARAM_VALID;
	}
	
	/* �ļ��������Ƿ��������Ѿ��ر� */
	if (iSockFd < 0)
	{
		return CBL_SEND_MESSAGE_MSG_SOCKET_VALID;
	}

	/* ������Ϣ */
	iSendRet = send(iSockFd, pcBuffer, iBufferLen, iFlags);
	
	return iSendRet;
}

/*******************************************************************************
*   �� ��   ����RecvMessage
*   ��     �ܣ�������Ϣ
*   ���������iSockFd:socket�ļ�������
*			  pcBuffer:������Ϣ��Ż���
*			  iBufferLen:������Ϣ���泤��
*			  iFlags:����ִ�з�ʽ
*   �����������
*   �� �� ֵ�����û�д����������ض�����ֽ���
*			  �����������ֹ������0
*			  ���򷵻�SOCKET_ERROR����Ӧ�ó����ͨ��WSAGetLastError()��ȡ��Ӧ������롣
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
INT32 RecvMessage(INT32 iSockFd, CHAR *pcBuffer, INT32 iBufferLen, INT32 iFlags)
{
	INT32 iRecvLen = 0;

	/* У����� */
	if (CBL_NULL == pcBuffer || iBufferLen <= 0)
	{
		return CBL_SEND_MESSAGE_MSG_PARAM_VALID;
	}
	
	/* �ļ��������Ƿ��������Ѿ��ر� */
	if (iSockFd < 0)
	{
		return CBL_SEND_MESSAGE_MSG_SOCKET_VALID;
	}

	/* ������Ϣ */
	iRecvLen = recv(iSockFd, pcBuffer, iBufferLen, iFlags);
	
	return iRecvLen;
}

/*******************************************************************************
*   �� ��   ����CreateUnixServerUDPSocket
*   ��     �ܣ�����Unix Socket��������
*   ���������pcSockPath:PATH�ļ�·��
*   �����������
*   �� �� ֵ��>0:socket�ļ�������
*             <0:����ʧ��
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-18
*   �޸���ʷ����
*******************************************************************************/
INT32 CreateUnixServerUDPSocket(CHAR *pcSockPath)
{
	INT32 iSockFd = -1;
	struct sockaddr_un stSockAddr;

	if (CBL_NULL == pcSockPath)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_PATH_VALID;
	}

	/* ����socket */
	iSockFd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (iSockFd < 0)
	{
		return CBL_CREATE_UNIX_SERVER_SOCKET_CREATE_FAILED;
	}

	/* ����socket��ַ */
	bzero(&stSockAddr, sizeof(stSockAddr));
	stSockAddr.sun_family = AF_UNIX;
	memcpy(stSockAddr.sun_path, pcSockPath, strlen(pcSockPath) + 1);

	/* socket���ַ�� */
	if (bind(iSockFd, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)) < 0)
	{  
		return CBL_CREATE_UNIX_SERVER_SOCKET_BIND_FAILED;
    }

	return iSockFd;
}

/*******************************************************************************
*   �� ��   ����CreateUnixClientUDPSocket
*   ��     �ܣ�����Unix Socket�ͻ���
*   ���������pcSockPath:PATH�ļ�·��
*   �����������
*   �� �� ֵ��>0:socket�ļ�������
*             <0:����ʧ��
*   ��     �ߣ�zhanxc
*   �������ڣ�018-7-21
*   �޸���ʷ����
*******************************************************************************/
INT32 CreateUnixClientUDPSocket(CHAR *pcSockPath)
{
	INT32 iSockFd = -1;
	INT32 iOnFlag = 1;
	struct sockaddr_un stServerAddr;

	if (CBL_NULL == pcSockPath)
	{
        return CBL_CREATE_UNIX_CLIENT_SOCKET_PATH_VALID;  
	}

	/* ����socket */
	if((iSockFd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {  
        return CBL_CREATE_UNIX_CLIENT_SOCKET_CREATE_FAILED;  
    }

	setsockopt(iSockFd, SOL_SOCKET, SO_REUSEADDR, &iOnFlag, sizeof(iOnFlag));

	/* ���õ�ַ */
	memset(&stServerAddr, 0, sizeof(stServerAddr));
	stServerAddr.sun_family = AF_UNIX;  
	memcpy(stServerAddr.sun_path, pcSockPath, strlen(pcSockPath) + 1);

	/* ���ӷ������� */
    if(connect(iSockFd, (struct sockaddr*)&stServerAddr, sizeof(stServerAddr)) < 0)
    {  
        return CBL_CREATE_UNIX_CLIENT_SOCKET_CONNECT_FAILED;  
    }

	return iSockFd;
}


