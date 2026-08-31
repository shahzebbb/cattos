#!/usr/bin/env bash
# We need more features bash provides and cannot rely on sh

# Set error modes for bash
# e = exit script when a command fails
# u = error if an unset variable is used
# -o pipefail = make a pipeline fail if any commands in it fails
set -euo pipefail

: "${TARGET:=i686-elf}"
: "${PREFIX:=/opt/cross}"
: "${BINUTILS_VERSION:=2.46.0}"
: "${GCC_VERSION:=16.1.0}"

export PATH="$PREFIX/bin:$PATH"

WORKDIR="/tmp/osdev-toolchain-build"
mkdir -p $WORKDIR "$PREFIX"
cd $WORKDIR

BINUTILS_TAR="binutils-${BINUTILS_VERSION}.tar.xz"
GCC_TAR="gcc-${GCC_VERSION}.tar.xz"

if [ ! -f "$BINUTILS_TAR" ]; then
    wget -q "https://ftp.gnu.org/gnu/binutils/$BINUTILS_TAR"
fi

if [ ! -f "$GCC_TAR" ]; then
    wget -q "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/$GCC_TAR"
fi

# Remove old tool chain builds just in case
rm -rf "binutils-${BINUTILS_VERSION}" "gcc-${GCC_VERSION}" build-binutils build-gcc

tar -xf "$BINUTILS_TAR"
tar -xf "$GCC_TAR"

mkdir build-binutils
cd build-binutils
../binutils-${BINUTILS_VERSION}/configure \
    --target="$TARGET" \
    --prefix="$PREFIX" \
    --with-sysroot \
    --disable-nls \
    --disable-werror
make -j$(nproc)
make install

cd $WORKDIR
mkdir build-gcc
cd build-gcc
../gcc-${GCC_VERSION}/configure \
    --target="$TARGET" \
    --prefix="$PREFIX" \
    --disable-nls \
    --enable-languages=c \
    --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc

cd /
rm -rf $WORKDIR
