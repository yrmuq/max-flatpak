# Map

- `ru.max.Unofficial.bundle.yaml` assembles the package and sets the sandbox permissions. It includes `max-deps.yaml`.
- `max-deps.yaml` builds the libraries the Flatpak runtime does not provide.
- `netlink_stub.c` is the preload used by `run_max.sh`. It covers the netlink bind and the lock workaround.
- `run_max.sh` starts the messenger, sets that preload, and selects the X11 platform.
- `.github/workflows/build.yml` downloads the RPM, builds the package, signs the update repository, deploys Pages, and then publishes the release.
- `pages-index.py` writes the directory page for the update site.
- `ru.max.Unofficial.desktop` and `ru.max.Unofficial.metainfo.xml` are the menu entry and the store description.
- `krb5.conf` and `krb5-1.22.patch.b64` configure and patch the bundled Kerberos library.
- `README.md` is the install and license text. `chit/lore.md` records decisions that the code does not make obvious.
