#!/usr/bin/env bash

# Arg 1:	URL for gcc sources
# Arg ...:	Configure script arguments
compile_gcc() {
	URL=$1
	shift 1 # We shift so we can access the rest of the arguments as $@

	# extract tar file name from the url string
	TAR_FILE=${URL##*/}
	GCC_FOLDER=${TAR_FILE%%.tar*}

	# Check if we already have the source
	if [ ! -d $GCC_FOLDER ]; then
		# Download and extract the source
		curl $URL --location | tar xjf -
	fi

	# Get GCC prerequisites
	cd $GCC_FOLDER
	./contrib/download_prerequisites
	cd ..

	# We do an out of source build
	mkdir build
	cd build

	../$GCC_FOLDER/configure $@

	make
	make install

	cd ..

	# Clean up
	#rm -rf build $GCC_FOLDER
}

# GCC_VER=4.9.2
GCC_VER=$1
shift 1

GCC_SOURCE_URL=http://ftpmirror.gnu.org/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2

compile_gcc $GCC_SOURCE_URL $@
