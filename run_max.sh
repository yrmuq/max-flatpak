#!/bin/sh
set -eu

MAX_BIN=/app/extra/max/bin/max

if [ ! -x "$MAX_BIN" ]; then
    echo "Error: MAX binary not found at $MAX_BIN" >&2
    echo "Searched tree:" >&2
    find /app/extra /app/opt -maxdepth 6 \( -type f -o -type l \) 2>/dev/null | head -n 40 >&2 || true
    exit 1
fi

# This MAX build is Qt and only speaks X11. On Wayland it runs through XWayland.
if [ -z "${QT_QPA_PLATFORM:-}" ]; then
    export QT_QPA_PLATFORM=xcb
fi

exec "$MAX_BIN" "$@"
