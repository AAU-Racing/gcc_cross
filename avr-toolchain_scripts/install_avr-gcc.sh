#!/bin/bash

BASEDIR=$(dirname $0) # Dir where the script is located

# GCC_VER=4.9.2
GCC_VER=$1
PREFIX=$2

# Check if how much total system memory we have
if [ "$(uname)" = "Darwin" ]; then
	# We are on Mac
	TOTAL=$(($(sysctl hw.memsize | awk '{ print $2 }')/(1024*1024)))
elif [ "$(uname)" = "Linux" ]; then
	# We are on Linux
	TOTAL=$(free -m | grep Mem  | awk '{ print $2 }')
fi

# We need more than two gb ram before to compile with LTO
if [ $TOTAL -gt 1999 ]; then
	LTO="--enable-lto"
fi

sh $BASEDIR/../install_gcc.sh $GCC_VER \
		--enable-languages=c,c++ \
		--target=avr \
		--disable-nls \
		--disable-libssp \
		--with-avrlibc \
		--prefix=$PREFIX \
		$LTO \
