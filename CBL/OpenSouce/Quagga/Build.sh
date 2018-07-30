#!/bin/bash
USAGE="Usage: [build.sh -b] or [build.sh -c], for build or clean quagga"
#获取当前目录路径
ROOT_DIR=$(cd `dirname $0`; pwd)
QUAGGA_TOP=$ROOT_DIR/quagga-1.2.4
QUAGGA_PREFIX=$ROOT_DIR/build/quagga
ENV_PATH=$ROOT_DIR/../Env.sh

#包含编译环境脚本文件
source $ENV_PATH

echo "Entering directory $QUAGGA_TOP ..."

case $1 in
        -b)
                mkdir $ROOT_DIR/build
                mkdir $QUAGGA_PREFIX
                tar -xzvf quagga-1.2.4.tar.gz
		cd $QUAGGA_TOP
                ./configure --prefix=$QUAGGA_PREFIX
                make
                make install
                ;;
        -c)
                rm -rf $QUAGGA_PREFIX
                rm -rf $ROOT_DIR/build
                rm -rf $QUAGGA_TOP
                ;;
        *)
                echo "Unknown parameters"
                echo $USAGE
                exit 1
                ;;
esac

