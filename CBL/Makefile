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

#定义Test目录
TEST := Test
TEST_SRC := $(ROOT)/$(TEST)
TEST_BUILD_PATH := $(BUILD_PATH)/$(TEST)

#编译参数
CFLAGS := -Wall -O2 -g -I $(BASE_INC)

################################################################################
all: $(MKDIR) $(BASE_LIB) $(TEST)
################################################################################

#创建文件夹
$(MKDIR):
	mkdir -p $(ROOT)/$(BUILD_PATH)		\
	$(ROOT)/$(BASE_BUILD_PATH)		\
	$(ROOT)/$(TEST_BUILD_PATH)

################################################################################
#链接Base库
$(BASE_LIB):	$(BASE_LIB)_objs
	$(CC) $(CFLAGS) -fPIC -shared -o $(BASE_BUILD_PATH)/$(BASE_LIB) $(BASE_BUILD_PATH)/List.o \
		$(BASE_BUILD_PATH)/Queue.o \
		$(BASE_BUILD_PATH)/Stack.o \
		$(BASE_BUILD_PATH)/HashTable.o \
		$(BASE_BUILD_PATH)/Btree.o

#编译Base
$(BASE_LIB)_objs:
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/List.c -o $(BASE_BUILD_PATH)/List.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Queue.c -o $(BASE_BUILD_PATH)/Queue.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Stack.c -o $(BASE_BUILD_PATH)/Stack.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/HashTable.c -o $(BASE_BUILD_PATH)/HashTable.o
	$(CC) $(CFLAGS) -fPIC -c $(BASE_SRC)/Btree.c -o $(BASE_BUILD_PATH)/Btree.o

################################################################################
#链接Test
$(TEST):	$(TEST)_objs
	$(CC) $(CFLAGS) -o $(TEST_BUILD_PATH)/$(TEST) $(TEST_BUILD_PATH)/main.o -L $(BASE_LIB_PATH) -l$(BASE_LIB_NAME)

#编译Test
$(TEST)_objs:
	$(CC) $(CFLAGS) -c $(TEST_SRC)/main.c -o $(TEST_BUILD_PATH)/main.o

################################################################################
#清除编译内容
clean:
	$(RM) $(BUILD_PATH)


