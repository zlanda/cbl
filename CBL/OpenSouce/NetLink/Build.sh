#!/bin/bash
USAGE="Usage: [build.sh -b] or [build.sh -c], for build or clean NetLink"
#获取当前目录路径
ROOT_DIR=$(cd `dirname $0`; pwd)
NETLINK_TOP=$ROOT_DIR/libnl-3.2.25
NETLINK_PREFIX=$ROOT_DIR/build/netlink
ENV_PATH=$ROOT_DIR/../Env.sh

#包含编译环境脚本文件
source $ENV_PATH

echo "Entering directory $NETLINK_TOP ..."

case $1 in
        -b)
                mkdir $ROOT_DIR/build
                mkdir $NETLINK_PREFIX
                tar -xzvf libnl-3.2.25.tar.gz
                cd $NETLINK_TOP
                ./configure --prefix=$NETLINK_PREFIX
                make
                make install
                ;;
        -c)
                rm -rf $NETLINK_PREFIX
                rm -rf $ROOT_DIR/build
                rm -rf $NETLINK_TOP
                ;;
        *)
                echo "Unknown parameters"
                echo $USAGE
                exit 1
                ;;
esac

