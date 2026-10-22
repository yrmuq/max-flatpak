#!/bin/sh
set -eu

MAX_BIN=/app/opt/max/bin/max

if [ ! -x "$MAX_BIN" ]; then
    echo "Error: MAX binary not found at $MAX_BIN" >&2
    exit 1
fi

if [ -z "${QT_QPA_PLATFORM:-}" ]; then
    export QT_QPA_PLATFORM=xcb
fi

# Host session manager is not reachable from the sandbox.
unset SESSION_MANAGER || true

if [ -f /app/lib/libnetlink-stub.so ]; then
    if [ -n "${LD_PRELOAD:-}" ]; then
        export LD_PRELOAD="/app/lib/libnetlink-stub.so:$LD_PRELOAD"
    else
        export LD_PRELOAD=/app/lib/libnetlink-stub.so
    fi
fi

exec "$MAX_BIN" "$@"
