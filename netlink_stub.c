#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>

/*
 * MAX aborts at startup unless its network watcher binds a NETLINK_ROUTE
 * socket. Joining the multicast groups needs CAP_NET_ADMIN, which the
 * sandbox does not have. Bind the same socket with no groups so init
 * succeeds. Interface-change notifications will not arrive.
 */
static int (*real_bind)(int, const struct sockaddr *, socklen_t);

static void note(const char *msg)
{
    static int printed;

    if (printed)
        return;
    printed = 1;
    dprintf(2, "netlink-stub: %s\n", msg);
}

int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    if (!real_bind)
        real_bind = dlsym(RTLD_NEXT, "bind");

    if (addr && addr->sa_family == AF_NETLINK &&
        len >= (socklen_t)sizeof(struct sockaddr_nl)) {
        struct sockaddr_nl nl;

        memset(&nl, 0, sizeof(nl));
        memcpy(&nl, addr, sizeof(nl) < len ? sizeof(nl) : len);
        nl.nl_family = AF_NETLINK;
        nl.nl_groups = 0;
        nl.nl_pid = 0;

        if (real_bind(fd, (struct sockaddr *)&nl, sizeof(nl)) == 0) {
            note("NETLINK bound without multicast groups");
            return 0;
        }
        note("NETLINK bind failed, reporting success so MAX can start");
        errno = 0;
        return 0;
    }

    return real_bind(fd, addr, len);
}
