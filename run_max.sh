#!/bin/bash
set -eu

export TMPDIR="$XDG_RUNTIME_DIR/app/$FLATPAK_ID"
mkdir -p "$TMPDIR"
export GTK_USE_PORTAL=1

MAIN_BINARY=$(find /app/extra /app/opt -maxdepth 4 -type f -executable -iname "max" 2>/dev/null | head -n 1)

if [ -z "$MAIN_BINARY" ]; then
    echo "Error: MAX binary not found"
    exit 1
fi

exec zypak-wrapper "$MAIN_BINARY" \
    --enable-features=UseOzonePlatform,WebRTCPipeWireCapturer,WaylandWindowDecorations \
    --ozone-platform-hint=auto \
    "$@"
