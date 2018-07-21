#编译动态库名称
BASE_LIB := libBasecbl.so
BASE_LIB_NAME := Basecbl

#Makefile中用到的命令
MKDIR := mkdir
RM := rm -rf

#命令
CC := gcc
CK := g++
LD := ld
OC := objcopy
OD := objdump
DD := dd
ELF := readelf

#定义编译根目录位置
ROOT := .

BUILD_PATH := $(ROOT)/build

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

#编译参数
CFLAGS := -Wall -O2 -g -I $(BASE_INC) -I $(RPC_INC) -I $(COMMON_INC)

################################################################################
all: $(MKDIR) $(BASE_LIB) $(TEST) $(CREATE_FIFO) $(READ_FIFO) $(WRITE_FIFO) $(CREATE_SHMID) \
	$(READ_SHMID) $(WRITE_SHMID) $(SEND_MSGQ) $(RECV_MSGQ) $(SEND_SOCK) $(RECV_SOCK)
################################################################################

#创建文件夹
$(MKDIR):
	mkdir -p $(ROOT)/$(BUILD_PATH)		\
	$(ROOT)/$(BASE_BUILD_PATH)		\
	$(ROOT)/$(RPC_BUILD_PATH)		\
	$(ROOT)/$(COMMON_BUILD_PATH)		\
	$(ROOT)/$(TEST_BUILD_PATH)

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
		$(COMMON_BUILD_PATH)/Lock.o \
		$(RPC_BUILD_PATH)/Pipe.o \
		$(RPC_BUILD_PATH)/Fifo.o \
		$(RPC_BUILD_PATH)/Shmid.o \
		$(RPC_BUILD_PATH)/Sem.o \
		$(RPC_BUILD_PATH)/Msqid.o \
		$(RPC_BUILD_PATH)/Socket.o \
		-lpthread

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
	$(CC) $(CFLAGS) -fPIC -c $(COMMON_SRC)/Lock.c -o $(COMMON_BUILD_PATH)/Lock.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Pipe.c -o $(RPC_BUILD_PATH)/Pipe.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Fifo.c -o $(RPC_BUILD_PATH)/Fifo.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Shmid.c -o $(RPC_BUILD_PATH)/Shmid.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Sem.c -o $(RPC_BUILD_PATH)/Sem.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Msqid.c -o $(RPC_BUILD_PATH)/Msqid.o
	$(CC) $(CFLAGS) -fPIC -c $(RPC_SRC)/Socket.c -o $(RPC_BUILD_PATH)/Socket.o

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

################################################################################
#清除编译内容
clean:
	$(RM) $(BUILD_PATH)


