# Protected Areas

- Do not add an AppImage build. The repository exists to keep the proprietary messenger inside a Flatpak sandbox, and an AppImage would not provide that isolation.
- Do not change the application id `ru.max.Unofficial`, the Flatpak branch `master`, the remote name `max-unofficial`, or the update-signing key. Installed clients use them to receive later versions.

# Required Behavior

- Ship the official MAX RPM inside the Flatpak. Do not publish a build that downloads the messenger onto the user's computer.
- Create the release only after the update repository is signed and GitHub Pages has deployed. A later scheduled run must rebuild when the release has no `ru.max.Unofficial.flatpakref`.
- Check the upstream package once a day. Do not poll more often.
- A commit that only updates a bundled library or a GitHub Action must not publish a release. Those updates are included in the next build of a new MAX package.
- Keep login data in the application's own data directory and allow both the freedesktop secret service and KWallet, so GNOME and KDE can restore the session.
- Keep access to camera and microphone device nodes. Calls need them, and removing that access breaks a call without opening the user's files.
- License the packaging files as GPL-3. The MAX program itself stays the proprietary upstream binary.

# Compatibility Workarounds

- `netlink_stub.c` binds the route socket without multicast groups. The sandbox cannot grant `CAP_NET_ADMIN`, and MAX aborts when that bind fails. Remove the stub if upstream stops requiring the socket. Interface-change notices do not arrive while it is used.
- The same library turns error-checking mutexes into recursive ones, reports a same-thread `EDEADLK` as success, and retries a deadlocked file lock. MAX aborts on that error instead of catching it. Remove this if upstream stops aborting, or if the workaround itself leaves a lock stuck.
- `run_max.sh` forces the `xcb` platform and clears `SESSION_MANAGER`. The official package has no Wayland plugin, and the host session manager is outside the sandbox. Drop the `xcb` force if upstream ships a Wayland plugin.
- The build links `libpulsecommon-14.0.so` to the path the call binary expects. Remove the link if upstream ships that file there.
- `max-deps.yaml` builds `libXaw`, `libXmu`, `libXRes`, `libfontenc`, `font-util`, and `krb5` because the binary needs them and the Flatpak runtime does not include them. Drop a library when a newer RPM no longer links it. The Kerberos ticket cache is set to `KCM:`, and that daemon is not running, so Kerberos login is not expected to work.

# External Compatibility Constraints

- Package only the official x86_64 RPM. Upstream publishes no ARM build, so do not add an arm64 Flatpak until that package exists.
- Use the upstream package version as the release tag. Do not invent a separate package version.
