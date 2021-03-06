#编译动态库名称
BASE_LIB := libBasecbl.so
BASE_LIB_NAME := Basecbl

#Makefile中用到的命令
MKDIR := mkdir
RM := rm -rf
CD := cd

#定义编译根目录位置
ROOT := .
BUILD_PATH := $(ROOT)/build

#包含编译环境脚本文件
include $(ROOT)/Env.sh

#定义编译Base目录变量
BASE := Base
BASE_SRC := $(ROOT)/$(BASE)/src
BASE_INC := $(ROOT)/$(BASE)/include
BASE_BUILD_PATH := $(BUILD_PATH)/$(BASE)
BASE_LIB_PATH := $(BUILD_PATH)/$(BASE)

#定义编译RPC目录变量
RPC := RPC
RPC_SRC := $(ROOT)/$(RPC)/src
RPC_INC := $(ROOT)/$(RPC)/include
RPC_BUILD_PATH := $(BUILD_PATH)/$(RPC)

#定义编译Common目录变量
COMMON := Common
COMMON_SRC := $(ROOT)/$(COMMON)/src
COMMON_INC := $(ROOT)/$(COMMON)/include
COMMON_BUILD_PATH := $(BUILD_PATH)/$(COMMON)

#定义Test目录
TEST := Test
TEST_SRC := $(ROOT)/$(TEST)
TEST_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义Socket目录
SOCKET_BUILD_PATH := $(BUILD_PATH)/Socket
SOCKETTEST_BUILD_PATH := $(BUILD_PATH)/SocketTest

#定义NetLinkTest目录
NETLINKTEST_BUILD_PATH := $(BUILD_PATH)/NetLinkTest

################################################################################
#定义创建FIFO目录
CREATE_FIFO := CreateFifo
CREATE_FIFO_SRC := $(ROOT)/$(TEST)
CREATE_FIFO_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义读FIFO目录
READ_FIFO := ReadFifo
READ_FIFO_SRC := $(ROOT)/$(TEST)
READ_FIFO_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义写FIFO目录
WRITE_FIFO := WriteFifo
WRITE_FIFO_SRC := $(ROOT)/$(TEST)
WRITE_FIFO_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义读共享内存目录
READ_SHMID := ReadShm
READ_SHMID_SRC := $(ROOT)/$(TEST)
READ_SHMID_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义写共享内存目录
WRITE_SHMID := WriteShm
WRITE_SHMID_SRC := $(ROOT)/$(TEST)
WRITE_SHMID_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义消息队列发送目录
SEND_MSGQ := SendMsgQ
SEND_MSGQ_SRC := $(ROOT)/$(TEST)
SEND_MSGQ_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义消息队列接收目录
RECV_MSGQ := RecvMsgQ
RECV_MSGQ_SRC := $(ROOT)/$(TEST)
RECV_MSGQ_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义socket发送目录
SEND_SOCK := SockSend
SEND_SOCK_SRC := $(ROOT)/$(TEST)
SEND_SOCK_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义socket接收目录
RECV_SOCK := SockRecv
RECV_SOCK_SRC := $(ROOT)/$(TEST)
RECV_SOCK_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义Epoll Server目录
EPOLL_SERVER := EpollServer
EPOLL_SERVER_SRC := $(ROOT)/$(TEST)
EPOLL_SERVER_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义Epoll Client目录
EPOLL_CLIENT := EpollClient
EPOLL_CLIENT_SRC := $(ROOT)/$(TEST)
EPOLL_CLIENT_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义Select Server目录
SELECT_SERVER := SelectServer
SELECT_SERVER_SRC := $(ROOT)/$(TEST)
SELECT_SERVER_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#定义Select Client目录
SELECT_CLIENT := SelectClient
SELECT_CLIENT_SRC := $(ROOT)/$(TEST)
SELECT_CLIENT_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义Signal处理目录
SIGNAL_TEST := SignalTest
SIGNAL_TEST_SRC := $(ROOT)/$(TEST)
SIGNAL_TEST_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义Timer处理目录
TIMER_TEST := TimerTest
TIMER_TEST_SRC := $(ROOT)/$(TEST)
TIMER_TEST_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义Thread测试目录
PTHREAD_TEST := ThreadTest
PTHREAD_TEST_SRC := $(ROOT)/$(TEST)
PTHREAD_TEST_BUILD_PATH := $(BUILD_PATH)/$(TEST)
################################################################################

################################################################################
#定义TcpServer测试目录
TCPSERVER_TEST := TcpServer
TCPSERVER_TEST_SRC := $(ROOT)/Socket
TCPSERVER_TEST_BUILD_PATH := $(BUILD_PATH)/Socket
################################################################################

################################################################################
#定义TcpClient测试目录
TCPCLIENT_TEST := TcpClient
TCPCLIENT_TEST_SRC := $(ROOT)/SocketTest
TCPCLIENT_TEST_BUILD_PATH := $(BUILD_PATH)/SocketTest
################################################################################

################################################################################
#定义UdpServer测试目录
UDPSERVER_TEST := UdpServer
UDPSERVER_TEST_SRC := $(ROOT)/Socket
UDPSERVER_TEST_BUILD_PATH := $(BUILD_PATH)/Socket
################################################################################

################################################################################
#定义UdpClient测试目录
UDPCLIENT_TEST := UdpClient
UDPCLIENT_TEST_SRC := $(ROOT)/SocketTest
UDPCLIENT_TEST_BUILD_PATH := $(BUILD_PATH)/SocketTest
################################################################################

################################################################################
#定义RawServer测试目录
RAWSERVER_TEST := RawServer
RAWSERVER_TEST_SRC := $(ROOT)/Socket
RAWSERVER_TEST_BUILD_PATH := $(BUILD_PATH)/Socket
################################################################################

################################################################################
#定义NetLinkTest测试目录
NETLINK_USER_TEST := NetLinkUserTest
NETLINK_USER_TEST_SRC := $(ROOT)/NetLinkTest
NETLINK_USER_TEST_BUILD_PATH := $(BUILD_PATH)/NetLinkTest
################################################################################

################################################################################
#定义内核HelloWorld模块
HELLOWORLD_TEST := HelloWorld
HELLOWORLD_DIR := KernelModeule/HelloWorld
HELLOWORLD_TEST_SRC := $(ROOT)/$(HELLOWORLD_DIR)
################################################################################

################################################################################
#定义内核Cdev模块
CDEV_MODULE := Cdev
CDEV_MODULE_DIR := KernelModeule/$(CDEV_MODULE)
CDEV_MODULE_SRC := $(ROOT)/$(CDEV_MODULE_DIR)
#定义内核Cdev测试目录
CDEV_TEST := CdevTest
CDEV_TEST_SRC := $(ROOT)/KernelTest
CDEV_TEST_BUILD_PATH := $(BUILD_PATH)/KernelTest
################################################################################

################################################################################
#定义内核IoctlTest模块
IOCTLTEST_MODULE := IoctlTest
IOCTLTEST_MODULE_DIR := KernelModeule/$(IOCTLTEST_MODULE)
IOCTLTEST_MODULE_SRC := $(ROOT)/$(IOCTLTEST_MODULE_DIR)
#定义内核Cdev测试目录
IOCTLTEST_TEST := IoctlAppTest
IOCTLTEST_TEST_SRC := $(ROOT)/KernelTest
IOCTLTEST_TEST_BUILD_PATH := $(BUILD_PATH)/KernelTest
################################################################################

################################################################################
#定义内核KthreadTest模块
KTHREAD_TEST := KthreadTest
KTHREAD_DIR := KernelModeule/Kthread
KTHREAD_TEST_SRC := $(ROOT)/$(KTHREAD_DIR)
################################################################################

################################################################################
#定义内核NotifierTest模块
NOTIFIER_TEST := NotifierTest
NOTIFIER_DIR := KernelModeule/NotifierChain
NOTIFIER_TEST_SRC := $(ROOT)/$(NOTIFIER_DIR)
################################################################################

################################################################################
#定义内核LockTest模块
LOCK_TEST := LockTest
LOCK_DIR := KernelModeule/Lock
LOCK_TEST_SRC := $(ROOT)/$(LOCK_DIR)
################################################################################

################################################################################
#定义内核NetlinkTest模块
NETLINK_KERNEL_TEST := NetLinkKernelTest
NETLINK_KERNEL_DIR := KernelModeule/NetLink
NETLINK_KERNEL_TEST_SRC := $(ROOT)/$(NETLINK_KERNEL_DIR)
################################################################################

#编译参数
CFLAGS := -Wall -O2 -g -I $(BASE_INC) -I $(RPC_INC) -I $(COMMON_INC)

################################################################################
all: $(MKDIR) $(BASE_LIB) $(TEST) $(CREATE_FIFO) $(READ_FIFO) $(WRITE_FIFO) $(CREATE_SHMID) \
	$(READ_SHMID) $(WRITE_SHMID) $(SEND_MSGQ) $(RECV_MSGQ) $(SEND_SOCK) $(RECV_SOCK) \
	$(TCPSERVER_TEST) $(TCPCLIENT_TEST) $(UDPSERVER_TEST) $(UDPCLIENT_TEST) $(RAWSERVER_TEST) $(NETLINK_USER_TEST) \
	$(EPOLL_SERVER) $(EPOLL_CLIENT) $(SELECT_SERVER) $(SELECT_CLIENT) $(SIGNAL_TEST) $(TIMER_TEST) \
	$(PTHREAD_TEST) $(CDEV_TEST) $(IOCTLTEST_TEST) $(HELLOWORLD_TEST) $(CDEV_MODULE) $(IOCTLTEST_MODULE) \
	$(KTHREAD_TEST) $(NOTIFIER_TEST) $(LOCK_TEST) $(NETLINK_KERNEL_TEST)
################################################################################

#创建文件夹
$(MKDIR):
	mkdir -p $(ROOT)/$(BUILD_PATH)		\
	$(ROOT)/$(BASE_BUILD_PATH)		\
	$(ROOT)/$(RPC_BUILD_PATH)		\
	$(ROOT)/$(COMMON_BUILD_PATH)		\
	$(ROOT)/$(SOCKET_BUILD_PATH)		\
	$(ROOT)/$(SOCKETTEST_BUILD_PATH)	\
	$(ROOT)/$(NETLINKTEST_BUILD_PATH)	\
	$(ROOT)/$(TEST_BUILD_PATH)			\
	$(ROOT)/$(CDEV_TEST_BUILD_PATH)

################################################################################
#链接Base库
$(BASE_LIB):	$(BASE_LIB)_objs
	$(CC) $(CFLAGS) -fPIC -shared -o $(BASE_BUILD_PATH)/$(BASE_LIB) $(BASE_BUILD_PATH)/List.o \
		$(BASE_BUILD_PATH)/Queue.o \
		$(BASE_BUILD_PATH)/Stack.o \
		$(BASE_BUILD_PATH)/HashTable.o \
		$(BASE_BUILD_PATH)/Btree.o \
		$(BASE_BUILD_PATH)/Tree.o \
		$(COMMON_BUILD_PATH)/Process.o \
		$(COMMON_BUILD_PATH)/Thread.o \
		$(COMMON_BUILD_PATH)/Timer.o \
		$(RPC_BUILD_PATH)/Pipe.o \
		$(RPC_BUILD_PATH)/Fifo.o \
		$(RPC_BUILD_PATH)/Shmid.o \
		$(RPC_BUILD_PATH)/Sem.o \
		$(RPC_BUILD_PATH)/Msqid.o \
		$(RPC_BUILD_PATH)/Socket.o \
		$(RPC_BUILD_PATH)/Epoll.o \
		$(RPC_BUILD_PATH)/Signal.o \
		-lpthread -lrt

#编译Base
$(BASE_LIB)_objs:
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/List.c -o $(BASE_BUILD_PATH)/List.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Queue.c -o $(BASE_BUILD_PATH)/Queue.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Stack.c -o $(BASE_BUILD_PATH)/Stack.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/HashTable.c -o $(BASE_BUILD_PATH)/HashTable.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Btree.c -o $(BASE_BUILD_PATH)/Btree.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Tree.c -o $(BASE_BUILD_PATH)/Tree.o
	$(CC) $(CFLAGS) -fPIC -c $(COMMON_SRC)/Process.c -o $(COMMON_BUILD_PATH)/Process.o
	$(CC) $(CFLAGS) -fPIC -c $(COMMON_SRC)/Thread.c -o $(COMMON_BUILD_PATH)/Thread.o
	$(CC) $(CFLAGS) -fPIC -c $(COMMON_SRC)/Timer.c -o $(COMMON_BUILD_PATH)/Timer.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Pipe.c -o $(RPC_BUILD_PATH)/Pipe.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Fifo.c -o $(RPC_BUILD_PATH)/Fifo.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Shmid.c -o $(RPC_BUILD_PATH)/Shmid.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Sem.c -o $(RPC_BUILD_PATH)/Sem.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Msqid.c -o $(RPC_BUILD_PATH)/Msqid.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Socket.c -o $(RPC_BUILD_PATH)/Socket.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Epoll.c -o $(RPC_BUILD_PATH)/Epoll.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Signal.c -o $(RPC_BUILD_PATH)/Signal.o

################################################################################
#链接Test
$(TEST):	$(TEST)_objs
	$(CC) $(CFLAGS) -o $(TEST_BUILD_PATH)/$(TEST) $(TEST_BUILD_PATH)/main.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译Test
$(TEST)_objs:
	$(CC) $(CFLAGS) -c $(TEST_SRC)/main.c -o $(TEST_BUILD_PATH)/main.o

#测试Fifo
################################################################################
#链接CreateFifo
$(CREATE_FIFO):	$(CREATE_FIFO)_objs
	$(CC) $(CFLAGS) -o $(CREATE_FIFO_BUILD_PATH)/$(CREATE_FIFO) $(CREATE_FIFO_BUILD_PATH)/fifo_create.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译CreateFifo
$(CREATE_FIFO)_objs:
	$(CC) $(CFLAGS) -c $(CREATE_FIFO_SRC)/fifo_create.c -o $(CREATE_FIFO_BUILD_PATH)/fifo_create.o

#链接ReadFifo
$(READ_FIFO):	$(READ_FIFO)_objs
	$(CC) $(CFLAGS) -o $(READ_FIFO_BUILD_PATH)/$(READ_FIFO) $(READ_FIFO_BUILD_PATH)/fifo_read.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译ReadFifo
$(READ_FIFO)_objs:
	$(CC) $(CFLAGS) -c $(READ_FIFO_SRC)/fifo_read.c -o $(READ_FIFO_BUILD_PATH)/fifo_read.o

#链接WriteFifo
$(WRITE_FIFO):	$(WRITE_FIFO)_objs
	$(CC) $(CFLAGS) -o $(WRITE_FIFO_BUILD_PATH)/$(WRITE_FIFO) $(WRITE_FIFO_BUILD_PATH)/fifo_write.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译WriteFifo
$(WRITE_FIFO)_objs:
	$(CC) $(CFLAGS) -c $(WRITE_FIFO_SRC)/fifo_write.c -o $(WRITE_FIFO_BUILD_PATH)/fifo_write.o
################################################################################

#测试共享内存
################################################################################
#链接ReadShm
$(READ_SHMID):	$(READ_SHMID)_objs
	$(CC) $(CFLAGS) -o $(READ_SHMID_BUILD_PATH)/$(READ_SHMID) $(READ_SHMID_BUILD_PATH)/shmid_read.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译ReadShm
$(READ_SHMID)_objs:
	$(CC) $(CFLAGS) -c $(READ_SHMID_SRC)/shmid_read.c -o $(READ_SHMID_BUILD_PATH)/shmid_read.o

#链接WriteShm
$(WRITE_SHMID):	$(WRITE_SHMID)_objs
	$(CC) $(CFLAGS) -o $(WRITE_SHMID_BUILD_PATH)/$(WRITE_SHMID) $(WRITE_SHMID_BUILD_PATH)/shmid_write.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译WriteShm
$(WRITE_SHMID)_objs:
	$(CC) $(CFLAGS) -c $(WRITE_SHMID_SRC)/shmid_write.c -o $(WRITE_SHMID_BUILD_PATH)/shmid_write.o
################################################################################

#测试消息队列
################################################################################
#链接SendMsgQ
$(SEND_MSGQ):	$(SEND_MSGQ)_objs
	$(CC) $(CFLAGS) -o $(SEND_MSGQ_BUILD_PATH)/$(SEND_MSGQ) $(SEND_MSGQ_BUILD_PATH)/msgsend.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译SendMsgQ
$(SEND_MSGQ)_objs:
	$(CC) $(CFLAGS) -c $(SEND_MSGQ_SRC)/msgsend.c -o $(SEND_MSGQ_BUILD_PATH)/msgsend.o

#链接RecvMsgQ
$(RECV_MSGQ):	$(RECV_MSGQ)_objs
	$(CC) $(CFLAGS) -o $(RECV_MSGQ_BUILD_PATH)/$(RECV_MSGQ) $(RECV_MSGQ_BUILD_PATH)/msgrecv.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译RecvMsgQ
$(RECV_MSGQ)_objs:
	$(CC) $(CFLAGS) -c $(RECV_MSGQ_SRC)/msgrecv.c -o $(RECV_MSGQ_BUILD_PATH)/msgrecv.o
################################################################################

#测试socket消息通信
################################################################################
#链接SockSend
$(SEND_SOCK):	$(SEND_SOCK)_objs
	$(CC) $(CFLAGS) -o $(SEND_SOCK_BUILD_PATH)/$(SEND_SOCK) $(SEND_SOCK_BUILD_PATH)/socket_send.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译SendMsgQ
$(SEND_SOCK)_objs:
	$(CC) $(CFLAGS) -c $(SEND_SOCK_SRC)/socket_send.c -o $(SEND_SOCK_BUILD_PATH)/socket_send.o

#链接RecvMsgQ
$(RECV_SOCK):	$(RECV_SOCK)_objs
	$(CC) $(CFLAGS) -o $(RECV_SOCK_BUILD_PATH)/$(RECV_SOCK) $(RECV_SOCK_BUILD_PATH)/socket_recv.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译RecvMsgQ
$(RECV_SOCK)_objs:
	$(CC) $(CFLAGS) -c $(RECV_SOCK_SRC)/socket_recv.c -o $(RECV_SOCK_BUILD_PATH)/socket_recv.o
################################################################################

#测试Epoll
################################################################################
#链接EpollServer
$(EPOLL_SERVER):	$(EPOLL_SERVER)_objs
	$(CC) $(CFLAGS) -o $(EPOLL_SERVER_BUILD_PATH)/$(EPOLL_SERVER) $(EPOLL_SERVER_BUILD_PATH)/epoll_server.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译EpollServer
$(EPOLL_SERVER)_objs:
	$(CC) $(CFLAGS) -c $(EPOLL_SERVER_SRC)/epoll_server.c -o $(EPOLL_SERVER_BUILD_PATH)/epoll_server.o

#链接EpollClient
$(EPOLL_CLIENT):	$(EPOLL_CLIENT)_objs
	$(CC) $(CFLAGS) -o $(EPOLL_CLIENT_BUILD_PATH)/$(EPOLL_CLIENT) $(EPOLL_CLIENT_BUILD_PATH)/epoll_client.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译EpollClient
$(EPOLL_CLIENT)_objs:
	$(CC) $(CFLAGS) -c $(EPOLL_CLIENT_SRC)/epoll_client.c -o $(EPOLL_CLIENT_BUILD_PATH)/epoll_client.o
################################################################################

#测试Select
################################################################################
#链接SelectServer
$(SELECT_SERVER):	$(SELECT_SERVER)_objs
	$(CC) $(CFLAGS) -o $(SELECT_SERVER_BUILD_PATH)/$(SELECT_SERVER) $(SELECT_SERVER_BUILD_PATH)/select_server.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译SelectServer
$(SELECT_SERVER)_objs:
	$(CC) $(CFLAGS) -c $(SELECT_SERVER_SRC)/select_server.c -o $(SELECT_SERVER_BUILD_PATH)/select_server.o

#链接SelectClient
$(SELECT_CLIENT):	$(SELECT_CLIENT)_objs
	$(CC) $(CFLAGS) -o $(SELECT_CLIENT_BUILD_PATH)/$(SELECT_CLIENT) $(SELECT_CLIENT_BUILD_PATH)/select_client.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译SelectClient
$(SELECT_CLIENT)_objs:
	$(CC) $(CFLAGS) -c $(SELECT_CLIENT_SRC)/select_client.c -o $(SELECT_CLIENT_BUILD_PATH)/select_client.o
################################################################################

#测试Signal
################################################################################
#链接Signal
$(SIGNAL_TEST):	$(SIGNAL_TEST)_objs
	$(CC) $(CFLAGS) -o $(SIGNAL_TEST_BUILD_PATH)/$(SIGNAL_TEST) $(SIGNAL_TEST_BUILD_PATH)/signal_test.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译Signal
$(SIGNAL_TEST)_objs:
	$(CC) $(CFLAGS) -c $(SIGNAL_TEST_SRC)/signal_test.c -o $(SIGNAL_TEST_BUILD_PATH)/signal_test.o
################################################################################

#测试Timer
################################################################################
#链接Timer
$(TIMER_TEST):	$(TIMER_TEST)_objs
	$(CC) $(CFLAGS) -o $(TIMER_TEST_BUILD_PATH)/$(TIMER_TEST) $(TIMER_TEST_BUILD_PATH)/timer_test.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译Timer
$(TIMER_TEST)_objs:
	$(CC) $(CFLAGS) -c $(TIMER_TEST_SRC)/timer_test.c -o $(TIMER_TEST_BUILD_PATH)/timer_test.o
################################################################################

#测试Thread
################################################################################
#链接Thread
$(PTHREAD_TEST):	$(PTHREAD_TEST)_objs
	$(CC) $(CFLAGS) -o $(PTHREAD_TEST_BUILD_PATH)/$(PTHREAD_TEST) $(PTHREAD_TEST_BUILD_PATH)/pthread_test.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME) -lpthread

#编译Thread
$(PTHREAD_TEST)_objs:
	$(CC) $(CFLAGS) -c $(PTHREAD_TEST_SRC)/pthread_test.c -o $(PTHREAD_TEST_BUILD_PATH)/pthread_test.o
################################################################################

#测试TcpServer
################################################################################
#链接TcpServer
$(TCPSERVER_TEST):	$(TCPSERVER_TEST)_objs
	$(CC) $(CFLAGS) -o $(TCPSERVER_TEST_BUILD_PATH)/$(TCPSERVER_TEST) $(TCPSERVER_TEST_BUILD_PATH)/TcpServer.o

#编译TcpServer
$(TCPSERVER_TEST)_objs:
	$(CC) $(CFLAGS) -c $(TCPSERVER_TEST_SRC)/TcpServer.c -o $(TCPSERVER_TEST_BUILD_PATH)/TcpServer.o
################################################################################

#测试TcpClient
################################################################################
#链接TcpClient
$(TCPCLIENT_TEST):	$(TCPCLIENT_TEST)_objs
	$(CC) $(CFLAGS) -o $(TCPCLIENT_TEST_BUILD_PATH)/$(TCPCLIENT_TEST) $(TCPCLIENT_TEST_BUILD_PATH)/TcpClient.o

#编译TcpClient
$(TCPCLIENT_TEST)_objs:
	$(CC) $(CFLAGS) -c $(TCPCLIENT_TEST_SRC)/TcpClient.c -o $(TCPCLIENT_TEST_BUILD_PATH)/TcpClient.o
################################################################################

#测试UdpServer
################################################################################
#链接UdpServer
$(UDPSERVER_TEST):	$(UDPSERVER_TEST)_objs
	$(CC) $(CFLAGS) -o $(UDPSERVER_TEST_BUILD_PATH)/$(UDPSERVER_TEST) $(UDPSERVER_TEST_BUILD_PATH)/UdpServer.o

#编译UdpServer
$(UDPSERVER_TEST)_objs:
	$(CC) $(CFLAGS) -c $(UDPSERVER_TEST_SRC)/UdpServer.c -o $(UDPSERVER_TEST_BUILD_PATH)/UdpServer.o
################################################################################

#测试UdpClient
################################################################################
#链接UdpClient
$(UDPCLIENT_TEST):	$(UDPCLIENT_TEST)_objs
	$(CC) $(CFLAGS) -o $(UDPCLIENT_TEST_BUILD_PATH)/$(UDPCLIENT_TEST) $(UDPCLIENT_TEST_BUILD_PATH)/UdpClient.o

#编译UdpClient
$(UDPCLIENT_TEST)_objs:
	$(CC) $(CFLAGS) -c $(UDPCLIENT_TEST_SRC)/UdpClient.c -o $(UDPCLIENT_TEST_BUILD_PATH)/UdpClient.o
################################################################################

#测试RawServer
################################################################################
#链接RawServer
$(RAWSERVER_TEST):	$(RAWSERVER_TEST)_objs
	$(CC) $(CFLAGS) -o $(RAWSERVER_TEST_BUILD_PATH)/$(RAWSERVER_TEST) $(RAWSERVER_TEST_BUILD_PATH)/RawServer.o

#编译RawServer
$(RAWSERVER_TEST)_objs:
	$(CC) $(CFLAGS) -c $(RAWSERVER_TEST_SRC)/RawServer.c -o $(RAWSERVER_TEST_BUILD_PATH)/RawServer.o
################################################################################

#测试NetLinkUserTest
################################################################################
#链接NetLinkUserTest
$(NETLINK_USER_TEST):	$(NETLINK_USER_TEST)_objs
	$(CC) $(CFLAGS) -o $(NETLINK_USER_TEST_BUILD_PATH)/$(NETLINK_USER_TEST) $(NETLINK_USER_TEST_BUILD_PATH)/NetLinkTest.o

#编译NetLinkUserTest
$(NETLINK_USER_TEST)_objs:
	$(CC) $(CFLAGS) -c $(NETLINK_USER_TEST_SRC)/NetLinkTest.c -o $(NETLINK_USER_TEST_BUILD_PATH)/NetLinkTest.o
################################################################################

################################################################################
#链接CdevTest
$(CDEV_TEST):	$(CDEV_TEST)_objs
	$(CC) $(CFLAGS) -o $(CDEV_TEST_BUILD_PATH)/$(CDEV_TEST) $(CDEV_TEST_BUILD_PATH)/CdevTest.o -L $(CDEV_TEST_BUILD_PATH) -l$(BASE_LIB_NAME)

#编译CdevTest
$(CDEV_TEST)_objs:
	$(CC) $(CFLAGS) -c $(CDEV_TEST_SRC)/CdevTest.c -o $(CDEV_TEST_BUILD_PATH)/CdevTest.o
################################################################################

################################################################################
#链接IoctlTest
$(IOCTLTEST_TEST):	$(IOCTLTEST_TEST)_objs
	$(CC) $(CFLAGS) -o $(IOCTLTEST_TEST_BUILD_PATH)/$(IOCTLTEST_TEST) $(IOCTLTEST_TEST_BUILD_PATH)/IoctlTest.o -L $(CDEV_TEST_BUILD_PATH) -l$(BASE_LIB_NAME)

#编译IoctlTest
$(IOCTLTEST_TEST)_objs:
	$(CC) $(CFLAGS) -c $(IOCTLTEST_TEST_SRC)/IoctlTest.c -o $(IOCTLTEST_TEST_BUILD_PATH)/IoctlTest.o
################################################################################

################################################################################
#编译内核HelloWorld模块
$(HELLOWORLD_TEST):
	make -C $(HELLOWORLD_TEST_SRC)
################################################################################

################################################################################
#编译内核Cdev模块
$(CDEV_MODULE):
	make -C $(CDEV_MODULE_SRC)
################################################################################

################################################################################
#编译内核IoctlTest模块
$(IOCTLTEST_MODULE):
	make -C $(IOCTLTEST_MODULE_SRC)
################################################################################

################################################################################
#编译内核KthreadTest模块
$(KTHREAD_TEST):
	make -C $(KTHREAD_TEST_SRC)
################################################################################

################################################################################
#编译内核NotifierTest模块
$(NOTIFIER_TEST):
	make -C $(NOTIFIER_TEST_SRC)
################################################################################

################################################################################
#编译内核LockTest模块
$(LOCK_TEST):
	make -C $(LOCK_TEST_SRC)
################################################################################

################################################################################
#编译内核NetLinkTest模块
$(NETLINK_KERNEL_TEST):
	make -C $(NETLINK_KERNEL_TEST_SRC)
################################################################################

#清除编译内容
clean:
	#echo前加上@能在Makefile中执行并回显，不加@能执行但不回显
	@echo "clean......"
	$(RM) $(BUILD_PATH)
	#清空HelloWorld内核模块
	make -C $(HELLOWORLD_TEST_SRC) clean
	#清空Cdev内核模块
	make -C $(CDEV_MODULE_SRC) clean
	#清空IoctlTest内核模块
	make -C $(IOCTLTEST_MODULE_SRC) clean
	#清空KthreadTest内核模块
	make -C $(KTHREAD_TEST_SRC) clean
	#清空NotifierTest内核模块
	make -C $(NOTIFIER_TEST_SRC) clean
	#清空LockTest内核模块
	make -C $(LOCK_TEST_SRC) clean
	#清空NetLinkTest内核模块
	make -C $(NETLINK_KERNEL_TEST_SRC) clean

