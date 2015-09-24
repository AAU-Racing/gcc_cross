#!/usr/bin/env bash

# Run this file as `. avr-toolchain.sh` to have the toochain in your path

export TOOLCHAIN_ROOT=$PWD/arm-baremetal-toolchain
export PATH=$PATH:$TOOLCHAIN_ROOT/bin

# Set the vesions for each component of the toolchain
export BINUTILS_VER=2.25
export GCC_VER=5.1.0
export NEWLIB_VER=2.1.0
export GDB_VER=7.9.1
export OPEN_OCD_VER=0.9.0

export TARGET=arm-none-eabi
