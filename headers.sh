#!/bin/sh

# Stop the script if a command fails
set -e

# Set env variables from config
# Important to run in current shell otherwise
# the env variables will be lost
. ./config.sh

mkdir -p "$SYSROOT"

# Make each of our projects for example
# kernel, libc, etc.
for PROJECT in $SYSTEM_HEADER_PROJECTS; do
    (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
done
