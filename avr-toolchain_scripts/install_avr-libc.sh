#!/usr/bin/env bash

VERSION=$1
PREFIX=$2

URL=http://download.savannah.gnu.org/releases/avr-libc/avr-libc-$VERSION.tar.bz2

TAR_FILE=${URL##*/}
SOURCE_FOLDER=${TAR_FILE%%.tar*}

if [ ! -d $SOURCE_FOLDER ]; then
	curl $URL --location | tar xjf -
fi

cd $SOURCE_FOLDER

./configure \
	--build=$(./config.guess) \
	--prefix=$PREFIX \
	--host=avr

make && make install

cd ..

# Cleanup
#rm -rf $SOURCE_FOLDER

# We need to copy our stuff to where avr-gcc looks for it
cd $PREFIX
cp -r avr/ .

