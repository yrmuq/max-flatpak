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
