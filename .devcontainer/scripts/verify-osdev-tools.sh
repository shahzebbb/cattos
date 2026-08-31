#!/usr/bin/env bash
set -euo pipefail

export PATH="/opt/cross/bin:$PATH"

printf "\nCattos Dev tool check\n"
printf "=======================\n"

for tool in \
    i686-elf-gcc \
    i686-elf-ld \
    i686-elf-ar \
    grub-mkrescue \
    grub-file \
    xorriso \
    qemu-system-i386 \
    make \
    file
  do
    printf "%-20s" "$tool"
    command -v "$tool"
done

printf "\nVersions\n"
printf "========\n"
i686-elf-gcc --version | head -n 1
i686-elf-ld --version | head -n 1
grub-mkrescue --version | head -n 1
qemu-system-i386 --version | head -n 1
make --version | head -n 1

printf "\nlibgcc path\n"
printf "===========\n"
i686-elf-gcc -print-libgcc-file-name
