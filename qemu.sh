# #!/bin/sh
# set -e
# . ./iso.sh

# qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom cattos.iso -display curses -monitor unix:/tmp/cattos-qemu.sock,server,nowait

set -e

. ./iso.sh

tmux kill-session -t cattos 2>/dev/null || true

tmux new-session -s cattos \
    "exec qemu-system-i386 -cdrom cattos.iso -display curses"

