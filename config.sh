SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

# Check if default env variables are provided
# otherwise use make and output of default.sh
export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc

# PREFIX are where normal installed files like headers or docs are installed
export PREFIX=/usr
# EXEC PREFIX are where executables or binaries are installed
# Not using ${} because we do not need a clear boundary
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS="-O2 -g"
export CPPFLAGS=""

# Configure cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# We do this because elf compilers were built --without-headers so C doesn't
# know where to find them.
# Hence, we point it towards our INCLUDEDIR which is usually /usr/include
if echo "$HOST" | grep -Eq -- 'elf($|-)'; then
    export CC="$CC -isystem=$INCLUDEDIR"
fi
