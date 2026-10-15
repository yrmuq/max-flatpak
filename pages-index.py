#!/usr/bin/env python3
import html
import time
from pathlib import Path

root = Path("public_repo")


def human_size(n):
    value = float(n)
    for unit in ("", "K", "M", "G"):
        if value < 1024 or unit == "G":
            if unit == "":
                return str(int(value))
            return f"{value:.1f}{unit}"
        value /= 1024
    return str(n)


rows = [
    (
        "ru.max.Unofficial.flatpakref",
        "https://github.com/yrmuq/max-package/releases/latest/download/ru.max.Unofficial.flatpakref",
        "-",
        "-",
        "\u0444\u0430\u0439\u043b \u0434\u043b\u044f \u0430\u0432\u0442\u043e\u043e\u0431\u043d\u043e\u0432\u043b\u0435\u043d\u0438\u044f",
    ),
    (
        "MAX.flatpak",
        "https://github.com/yrmuq/max-package/releases/latest/download/MAX.flatpak",
        "-",
        "-",
        "\u043f\u043e\u043b\u043d\u044b\u0439 \u043f\u0430\u043a\u0435\u0442, \u0431\u0435\u0437 \u0430\u0432\u0442\u043e\u043e\u0431\u043d\u043e\u0432\u043b\u0435\u043d\u0438\u044f",
    ),
]

descriptions = {
    "config": "\u043d\u0430\u0441\u0442\u0440\u043e\u0439\u043a\u0438 \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0430",
    "summary": "\u0441\u043f\u0438\u0441\u043e\u043a \u0432\u0435\u0440\u0441\u0438\u0439",
    "summary.sig": "\u043f\u043e\u0434\u043f\u0438\u0441\u044c \u0441\u043f\u0438\u0441\u043a\u0430",
    ".nojekyll": "\u0441\u043b\u0443\u0436\u0435\u0431\u043d\u044b\u0439 \u0444\u0430\u0439\u043b",
}

for path in sorted(root.iterdir(), key=lambda item: item.name.lower()):
    if path.name == "index.html":
        continue
    stamp = time.strftime("%Y-%m-%d %H:%M", time.gmtime(path.stat().st_mtime))
    if path.is_dir():
        rows.append((path.name + "/", None, stamp, "-", "\u0444\u0430\u0439\u043b\u044b \u0434\u043b\u044f Flatpak, \u0432\u0440\u0443\u0447\u043d\u0443\u044e \u043d\u0435 \u0441\u043a\u0430\u0447\u0438\u0432\u0430\u0442\u044c"))
    else:
        rows.append((path.name, path.name, stamp, human_size(path.stat().st_size), descriptions.get(path.name, "")))

body = [
    '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">',
    "<html>",
    " <head>",
    "  <title>Index of /max-package</title>",
    " </head>",
    " <body>",
    "<h1>Index of /max-package</h1>",
    "<p>\u041a\u0430\u0442\u0430\u043b\u043e\u0433 \u043e\u0431\u043d\u043e\u0432\u043b\u0435\u043d\u0438\u0439 MAX. \u0421\u0430\u043c\u0443 \u043f\u0440\u043e\u0433\u0440\u0430\u043c\u043c\u0443 Flatpak \u0437\u0430\u0431\u0438\u0440\u0430\u0435\u0442 \u043e\u0442\u0441\u044e\u0434\u0430.</p>",
    "<table>",
    " <tr><th>Name</th><th>Last modified</th><th>Size</th><th>Description</th></tr>",
    ' <tr><th colspan="4"><hr></th></tr>',
]
for name, href, modified, size, description in rows:
    label = html.escape(name)
    shown = f'<a href="{html.escape(href, quote=True)}">{label}</a>' if href else label
    body.append(
        " <tr>"
        f"<td>{shown}</td>"
        f'<td align="right">{html.escape(modified)}&nbsp;</td>'
        f'<td align="right">{html.escape(size)}</td>'
        f"<td>{html.escape(description)}</td>"
        "</tr>"
    )
body.extend([
    ' <tr><th colspan="4"><hr></th></tr>',
    "</table>",
    "</body></html>",
    "",
])
(root / "index.html").write_text("\n".join(body), encoding="utf-8")
