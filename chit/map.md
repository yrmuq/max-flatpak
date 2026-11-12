# Map

- `ru.max.Unofficial.bundle.yaml` assembles the package and sets the sandbox permissions. It includes `max-deps.yaml`.
- `max-deps.yaml` builds the libraries the Flatpak runtime does not provide.
- `netlink_stub.c` is the preload used by `run_max.sh`. It covers the netlink bind and the lock workaround.
- `run_max.sh` starts the messenger, sets that preload, and selects the X11 platform.
- `.github/workflows/build.yml` downloads the RPM, builds the package, signs the update repository, deploys Pages, and then publishes the release.
- `pages-index.py` writes the directory page for the update site.
- `ru.max.Unofficial.desktop` and `ru.max.Unofficial.metainfo.xml` are the menu entry and the store description.
- `krb5.conf` sets the bundled Kerberos library to a local ticket cache that the sandbox does not run.
- `.github/dependabot.yml` and `.github/workflows/merge-updates.yml` bring GitHub Action updates into main on Fridays without publishing a release.
- `.github/workflows/update-libs.yml` and `scripts/bump-libs.py` do the same for the bundled libraries.
- `README.md` is the install and license text. `chit/lore.md` records decisions that the code does not make obvious.
