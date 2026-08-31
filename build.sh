#!/bin/sh

# Stop the script if a command fails
set -e

# Set env variables and copy headers
# Important to run in current shell otherwise
# the env variables will be lost
. ./headers.sh

# Make each of our projects for example
# kernel, libc, etc.
for PROJECT in $PROJECTS; do
    (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
