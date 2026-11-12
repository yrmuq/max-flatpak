#!/usr/bin/env python3
"""Bump bundled libraries inside their current series. Does not publish a release."""
import hashlib
import json
import re
import urllib.request
from pathlib import Path

YAML = Path("max-deps.yaml")
text = YAML.read_text()


def fetch(url):
    with urllib.request.urlopen(url, timeout=60) as response:
        return response.read()


def gitlab_project(url):
    path = url.removeprefix("https://gitlab.freedesktop.org/").removesuffix(".git")
    return urllib.request.quote(path, safe="")


def latest_tag(url, prefix):
    tags = json.loads(fetch(
        f"https://gitlab.freedesktop.org/api/v4/projects/{gitlab_project(url)}/repository/tags?per_page=100"
    ))
    best = None
    best_key = None
    for tag in tags:
        match = re.fullmatch(re.escape(prefix) + r"(\d+(?:\.\d+)*)", tag["name"])
        if not match:
            continue
        key = tuple(int(part) for part in match.group(1).split("."))
        if best_key is None or key > best_key:
            best_key = key
            best = tag
    return best


changed = False
for match in list(re.finditer(
    r"(url: (https://gitlab\.freedesktop\.org/\S+\.git)\n\s+commit: )([0-9a-f]+)(\n\s+tag: )(\S+)",
    text,
)):
    url, tag = match.group(2), match.group(5)
    prefix = re.sub(r"\d+(?:\.\d+)*$", "", tag)
    latest = latest_tag(url, prefix)
    if latest is None or latest["name"] == tag:
        continue
    replacement = f"{match.group(1)}{latest['commit']['id']}{match.group(4)}{latest['name']}"
    text = text.replace(match.group(0), replacement, 1)
    changed = True
    print(f"{tag} -> {latest['name']}")

current = re.search(
    r"url: (https://kerberos\.org/dist/krb5/(\d+\.\d+)/krb5-(\d+\.\d+\.\d+)\.tar\.gz)\n\s+sha256: ([0-9a-f]+)",
    text,
)
if current:
    series, version = current.group(2), current.group(3)
    page = fetch(f"https://kerberos.org/dist/krb5/{series}/").decode("utf-8", "replace")
    versions = sorted(
        {found for found in re.findall(rf"krb5-({re.escape(series)}\.\d+)\.tar\.gz", page)},
        key=lambda value: tuple(int(part) for part in value.split(".")),
    )
    # shortcut: stay on this krb5 series, a new series needs a compiler check
    if versions and versions[-1] != version:
        new_version = versions[-1]
        new_url = f"https://kerberos.org/dist/krb5/{series}/krb5-{new_version}.tar.gz"
        digest = hashlib.sha256(fetch(new_url)).hexdigest()
        text = text.replace(current.group(1), new_url, 1).replace(current.group(4), digest, 1)
        changed = True
        print(f"krb5 {version} -> {new_version}")

if changed:
    YAML.write_text(text)
else:
    print("no library updates")
