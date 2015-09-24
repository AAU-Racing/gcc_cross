#!/usr/bin/env bash

VERSION=$1
shift 1 # configure options is in $@

URL=http://ftp.gnu.org/gnu/binutils/binutils-$VERSION.tar.bz2

TAR_FILE=${URL##*/}
SOURCE_FOLDER=${TAR_FILE%%.tar*}

# Check if we already have the source
if [ ! -d $SOURCE_FOLDER ]; then
	curl $URL --location | tar xjf -
fi

cd $SOURCE_FOLDER

./configure $@

make && make install

cd ..

# Clean up
#rm -rf $SOURCE_FOLDER
