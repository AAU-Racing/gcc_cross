#!/usr/bin/env bash

# Stop if any command fails
set -e

BASEDIR=$(dirname $0) # Dir where the script is located

# We gcc and g++ is the c and c++ compiler
# makeinfo is required to build newlib. It comes with the package "texinfo" in
# ubuntu
if ! sh $BASEDIR/../bin_exists.sh gcc g++ makeinfo ; then
	exit 1
fi

# Set variables in our path for the duration of this script.
# This sets needed configuration variables
. $BASEDIR/arm-baremetal-toolchain_path.sh

N_CPUS=$(getconf _NPROCESSORS_ONLN)

# Create the folders that we will use
mkdir -p $TOOLCHAIN_ROOT

TOOL_BUILD_DIR=/tmp/toolchain_build
TOOL_SRC_DIR=/tmp/toolchain_src

mkdir -p $TOOL_BUILD_DIR
mkdir -p $TOOL_SRC_DIR


simple_installer() {
	URL=$1
	shift 1

	TMP=${URL%%/download*} # Some links appends this. Remove it to get the name
	TAR_FILE=${TMP##*/}
	SRC=${TAR_FILE%%.tar*}

	# Guess the compression type
	TAR_TYPE=${TAR_FILE##*tar.}
	case $TAR_TYPE in
		"gz")
			TAR_OPT=xzf
			;;
		"bz2")
			TAR_OPT=xjf
			;;
	esac

	# Download and extract the source into the source folder
	pushd $TOOL_SRC_DIR
	# Check if we already have the source
	if [ ! -d $SRC ]; then
		curl $URL --location --progress-bar | tar $TAR_OPT -
	fi
	popd

	# Now configure and install it
	mkdir -p $TOOL_BUILD_DIR/${SRC}_build
	pushd $TOOL_BUILD_DIR/${SRC}_build

	$TOOL_SRC_DIR/$SRC/configure $@

	make -j $N_CPUS && make install

	rm -rf $TAR_FILE $SRC

	popd
}

install_binutils() {
	VERSION=$1
	shift 1 # configure options is in $@

	URL=http://ftp.gnu.org/gnu/binutils/binutils-$VERSION.tar.bz2

	simple_installer $URL $@
}

# libc
install_newlib() {
	VERSION=$1
	shift 1

	URL=ftp://sourceware.org/pub/newlib/newlib-$VERSION.tar.gz

	simple_installer $URL $@
}

install_gdb() {
	VERSION=$1
	shift 1

	URL=http://ftp.gnu.org/gnu/gdb/gdb-$GDB_VER.tar.gz

	simple_installer $URL $@
}

install_open_ocd() {
	VERSION=$1
	shift 1

	URL=http://sourceforge.net/projects/openocd/files/openocd/$OPEN_OCD_VER/openocd-$OPEN_OCD_VER.tar.bz2/download

	simple_installer $URL $@
}


# Check how much total system memory we have
if [ "$(uname)" = "Darwin" ]; then
	# We are on Mac
	MEMORY=$(($(sysctl hw.memsize | awk '{ print $2 }')/(1024*1024)))
elif [ "$(uname)" = "Linux" ]; then
	# We are on Linux
	MEMORY=$(free -m | grep Mem  | awk '{ print $2 }')
fi

# We need more than two gb ram before to compile with LTO
if [ $MEMORY -gt 1999 ]; then
	LTO="--enable-lto"
fi

gcc_install() {
	GCC_VER=$1
	shift 1 # We shift so we can access the rest of the arguments as $@

	URL=http://ftpmirror.gnu.org/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.bz2

	# extract tar file name from the url string
	TAR_FILE=${URL##*/}
	SRC=${TAR_FILE%%.tar*}

	# Check if we already have the source
	pushd $TOOL_SRC_DIR
	if [ ! -d $SRC ]; then
		# Download and extract the source
		curl $URL --location | tar xjf -

		# Now get GCC prerequisites
		pushd $TOOL_SRC_DIR/$SRC
		./contrib/download_prerequisites
		popd
	fi
	popd

	# We do an out of source build

	mkdir -p $TOOL_BUILD_DIR/${SRC}_build
	pushd $TOOL_BUILD_DIR/${SRC}_build

	# First build an incomplete compiler that we can use to compile libc
	echo "Boostrapping gcc"
	$TOOL_SRC_DIR/$SRC/configure $@ --without-headers
	make -j $N_CPUS all-gcc
	make install-gcc

	make -j $N_CPUS -k all-target-libgcc # -k do as much as possible.
	make -j $N_CPUS -i install-target-libgcc # -i ignore errors.
	# There are some missing parts which makes make not to
	# install everything. With -i all available parts will be installed.

	popd

	# Now that we have partial compiler for our target we can build standard
	# C library (libc) that we want to use
	echo "installing newlib (libc)"
	install_newlib $NEWLIB_VER \
		--target=$TARGET \
		--enable-interwork \
		--enable-multilib \
		--enable-lto \
		--prefix=$TOOLCHAIN_ROOT \
		--disable-newlib-supplied-syscalls \
		--enable-newlib-nano-malloc \

	# Now that we have compiled libc we can build the rest of our compiler.
	# Build the rest of GCC
	pushd $TOOL_BUILD_DIR/${SRC}_build
	echo "Building the rest of gcc"
	$TOOL_SRC_DIR/$SRC/configure $@
	make -j $N_CPUS all
	make install

	echo "DONE BUILDING GCC"

	popd
}


install_binutils $BINUTILS_VER \
	--enable-lto \
	--disable-werror \
	--disable-nls \
	--prefix=$TOOLCHAIN_ROOT \
	--target=$TARGET \
	--enable-interwork \
	--enable-multilib \

gcc_install $GCC_VER \
	--target=$TARGET \
	--enable-interwork \
	--enable-multilib \
	--enable-languages="c,c++" \
	--with-newlib \
	--disable-nls \
	--with-system-zlib \
	--prefix=$TOOLCHAIN_ROOT \
	--with-cpu=cortex-m4 \
	--with-fpu=fpv4-sp-d16 \
	--with-float=hard \
	--with-mode=thumb \
	--disable-shared \
	$LTO

install_gdb $GDB_VER \
	--target=$TARGET \
	--prefix=$TOOLCHAIN_ROOT \
	--enable-interwork \
	--enable-multilib \
	--enable-lto \
	--disable-nls \

install_open_ocd $OPEN_OCD_VER \
	--prefix=$TOOLCHAIN_ROOT \
	--enable-stlink \
	--enable-maintainer-mode

# Create a readme file with info of library versions
touch $TOOLCHAIN_ROOT/README
echo "ARM baremetal toolchain:" > $TOOLCHAIN_ROOT/README
echo BINUTILS_VER=$BINUTILS_VER >> $TOOLCHAIN_ROOT/README
echo GCC_VER=$GCC_VER >> $TOOLCHAIN_ROOT/README
echo NEWLIB_VER=$NEWLIB_VER >> $TOOLCHAIN_ROOT/README
echo GDB_VER=$GDB_VER >> $TOOLCHAIN_ROOT/README
echo OPEN_OCD_VER=$OPEN_OCD_VER >> $TOOLCHAIN_ROOT/README

# Tar the toolchain so it is ready for distribution
#tar -jcvf arm-baremetal-toolchain_$(uname -ms | tr " /" _).tar.bz2 $TOOLCHAIN_ROOT

rm -rf $TOOL_BUILD_DIR $TOOL_SRC_DIR

cat >$BASEDIR/test.c <<EOL
#include <stdio.h>

int main(void) {
	printf("Hello, World!");
	return 0;
}
EOL
$TOOLCHAIN_ROOT/bin/arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mthumb-interwork $BASEDIR/test.c && rm a.out && echo "Successfully compiled test with the newly build cross compiler!"
rm $BASEDIR/test.c

# bash $BASEDIR/STM32/setup_stm32.sh $TOOLCHAIN_ROOT STM32F407xx # No longer usefull

echo "Done installing toolchain in $TOOLCHAIN_ROOT"
