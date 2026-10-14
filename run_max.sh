#!/bin/sh
set -eu

MAX_BIN=
for candidate in /app/opt/max/bin/max /app/extra/max/bin/max; do
    if [ -x "$candidate" ]; then
        MAX_BIN=$candidate
        break
    fi
done

if [ -z "$MAX_BIN" ]; then
    echo "Error: MAX binary not found" >&2
    echo "Tried /app/opt/max/bin/max and /app/extra/max/bin/max" >&2
    ls -la /app /app/opt /app/extra >&2 2>/dev/null || true
    exit 1
fi

# This MAX build is Qt and only speaks X11. On Wayland it runs through XWayland.
if [ -z "${QT_QPA_PLATFORM:-}" ]; then
    export QT_QPA_PLATFORM=xcb
fi

exec "$MAX_BIN" "$@"
