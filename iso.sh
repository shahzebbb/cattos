#!/bin/sh
set -e

. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/cattos.kernel  isodir/boot/cattos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "cattos" {
    multiboot /boot/cattos.kernel
}
EOF
grub-mkrescue -o cattos.iso isodir

