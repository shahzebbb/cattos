# #!/bin/sh
# set -e
# . ./iso.sh

# qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom cattos.iso -display curses -monitor unix:/tmp/cattos-qemu.sock,server,nowait

set -e

. ./iso.sh

tmux kill-session -t cattos 2>/dev/null || true

# Clear previous serial output
: > serial.log

# Start QEMU in the first pane but don't attach
tmux new-session -d -s cattos \
    "exec qemu-system-i386 \
        -cdrom cattos.iso \
        -display curses \
        -serial file:serial.log"

# Split window and follow serial log in new pane
tmux split-window -h -t cattos \
    "exec tail -f serial.log"

# Give both panes equal width
tmux select-layout -t cattos even-horizontal

# Put focus back on the QEMU pane
tmux select-pane -t cattos:0.0

# Attach to the fully configured session
tmux attach-session -t cattos


