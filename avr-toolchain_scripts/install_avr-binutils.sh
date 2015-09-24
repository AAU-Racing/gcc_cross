#!/usr/bin/env bash

VERSION=$1
PREFIX=$2

BASEDIR=$(dirname $0) # Dir where the script is located

sh $BASEDIR/../install_binutils.sh $VERSION \
	--enable-lto \
	--disable-werror \
	--disable-nls \
	--disable-install-libiberty \
	--prefix=$PREFIX \
	--target=avr
