#!/usr/bin/env bash

# Run this file as `. avr-toolchain.sh` to have the toochain in your path

export TOOLCHAIN_ROOT=$PWD/avr-toolchain
export PATH=$TOOLCHAIN_ROOT/bin:$PATH
