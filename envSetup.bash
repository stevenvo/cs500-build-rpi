#!/bin/csh
export BUILD_RPI_DIR=/home/vagrant/dev/buildRpi
export PATH=$PATH:${BUILD_RPI_DIR}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin
export CCPREFIX=${BUILD_RPI_DIR}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-
#
export TARGET=arm-linux-gnueabihf
#export PRJROOT=`pwd`
export PREFIX=${BUILD_RPI_DIR}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian
export INCPREFIX=${BUILD_RPI_DIR}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr
export SYSROOT=${PREFIX}/${TARGET}

unset LD_LIBRARY_PATH
