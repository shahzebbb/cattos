#!/bin/sh
# Regex on the first arg "$1" to check if it is in pattern i?86-,
# if it is then output i386
# Otherwise print whatever is the host for example x86_64-elf gets printed out
# as x86_63
if echo "$1" | grep -Eq "i[[:digit:]]86-"; then
    echo i386
else
    echo "$1" | grep -Eo "^[[:alnum:]_]*"
fi
