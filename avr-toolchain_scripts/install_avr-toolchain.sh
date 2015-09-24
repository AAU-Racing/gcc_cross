#!/usr/bin/env bash

BASEDIR=$(dirname $0) # Dir where the script is located

# That required binaries used for building is present in the path
if ! sh $BASEDIR/../bin_exists.sh gcc g++; then
	exit 1
fi

# Set variables in our path for the duration of this script
. $BASEDIR/avr-toolchain_path.sh

mkdir -p $TOOLCHAIN_ROOT

# Set the vesions for each component of the toolchain
BINUTILS_VER=2.24
GCC_VER=4.9.2
AVR_LIBC_VER=1.8.1

# Install each component
sh $BASEDIR/install_avr-binutils.sh $BINUTILS_VER $TOOLCHAIN_ROOT
sh $BASEDIR/install_avr-gcc.sh $GCC_VER $TOOLCHAIN_ROOT
sh $BASEDIR/install_avr-libc.sh $AVR_LIBC_VER $TOOLCHAIN_ROOT

# Compile the test program with the new toolchain to check that it works.
if ! $TOOLCHAIN_ROOT/bin/avr-gcc -std=c99 -mmcu=at90can128  -Wall -Os $BASEDIR/test.c -o test && rm test; then
	echo "Failed to compile test program"
	exit 1
fi

# Create a readme file with info of library versions
touch $TOOLCHAIN_ROOT/README
echo "AVR toolchain:" > $TOOLCHAIN_ROOT/README
echo BINUTILS_VER=$BINUTILS_VER >> $TOOLCHAIN_ROOT/README
echo GCC_VER=$GCC_VER >> $TOOLCHAIN_ROOT/README
echo AVR_LIBC_VER=$AVR_LIBC_VER >> $TOOLCHAIN_ROOT/README

# Tar the toolchain so it is ready for distribution
tar -jcvf avr-toolchain_$(uname -ms | tr " /" _).tar.bz2 $TOOLCHAIN_ROOT

