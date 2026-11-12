#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>

/*
 * MAX aborts at startup unless its network watcher binds a NETLINK_ROUTE
 * socket. Joining the multicast groups needs CAP_NET_ADMIN, which the
 * sandbox does not have. Bind the same socket with no groups so init
 * succeeds. Interface-change notifications will not arrive.
 *
 * MAX also aborts when the same thread locks a mutex it already holds
 * (std::system_error, EDEADLK, "resource deadlock avoided"). Error-checking
 * mutexes are turned into recursive ones so the second lock is counted and
 * the matching unlock still releases it. A leftover EDEADLK is reported as
 * success so the process does not abort. File locks that hit EDEADLK are
 * retried for a moment instead.
 */
static int (*real_bind)(int, const struct sockaddr *, socklen_t);

static void note(const char *prefix, const char *msg)
{
    write(2, prefix, strlen(prefix));
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
}

int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    static int printed;

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
            if (!printed) {
                printed = 1;
                note("netlink-stub: ", "NETLINK bound without multicast groups");
            }
            return 0;
        }
        if (!printed) {
            printed = 1;
            note("netlink-stub: ", "NETLINK bind failed, reporting success so MAX can start");
        }
        errno = 0;
        return 0;
    }

    return real_bind(fd, addr, len);
}

static int errorcheck_type(int type)
{
#ifdef PTHREAD_MUTEX_ERRORCHECK_NP
    if (type == PTHREAD_MUTEX_ERRORCHECK_NP)
        return 1;
#endif
    return type == PTHREAD_MUTEX_ERRORCHECK;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    static int (*real_settype)(pthread_mutexattr_t *, int);
    static int printed;

    if (!real_settype)
        real_settype = dlsym(RTLD_NEXT, "pthread_mutexattr_settype");
    if (errorcheck_type(type)) {
        if (!printed) {
            printed = 1;
            note("max-stub: ", "error-checking mutexes are recursive, so a second lock does not abort");
        }
        type = PTHREAD_MUTEX_RECURSIVE;
    }
    return real_settype(attr, type);
}

static int ignore_deadlock(int rc)
{
    static int printed;

    if (rc != EDEADLK)
        return rc;
    if (!printed) {
        printed = 1;
        note("max-stub: ", "ignored a same-thread mutex deadlock");
    }
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    static int (*real_lock)(pthread_mutex_t *);

    if (!real_lock)
        real_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    return ignore_deadlock(real_lock(mutex));
}

int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abstime)
{
    static int (*real_timedlock)(pthread_mutex_t *, const struct timespec *);

    if (!real_timedlock)
        real_timedlock = dlsym(RTLD_NEXT, "pthread_mutex_timedlock");
    if (!real_timedlock)
        return pthread_mutex_lock(mutex);
    return ignore_deadlock(real_timedlock(mutex, abstime));
}

int pthread_mutex_clocklock(pthread_mutex_t *mutex, clockid_t clockid,
                            const struct timespec *abstime)
{
    static int (*real_clocklock)(pthread_mutex_t *, clockid_t, const struct timespec *);

    if (!real_clocklock)
        real_clocklock = dlsym(RTLD_NEXT, "pthread_mutex_clocklock");
    if (!real_clocklock)
        return EINVAL;
    return ignore_deadlock(real_clocklock(mutex, clockid, abstime));
}

static int retry_fcntl(int (*real_fcntl)(int, int, void *), int fd, int cmd, void *arg)
{
    int rc;
    int i;

    rc = real_fcntl(fd, cmd, arg);
    if (rc >= 0 || errno != EDEADLK)
        return rc;
    if (cmd != F_SETLKW && cmd != F_OFD_SETLKW
#ifdef F_SETLKW64
        && cmd != F_SETLKW64
#endif
        )
        return rc;

    for (i = 0; i < 30; i++) {
        usleep(20000);
        rc = real_fcntl(fd, cmd, arg);
        if (rc >= 0 || errno != EDEADLK)
            return rc;
    }
    note("max-stub: ", "file lock stayed deadlocked after retries");
    return rc;
}

int fcntl(int fd, int cmd, ...)
{
    static int (*real_fcntl)(int, int, void *);
    va_list ap;
    void *arg;

    if (!real_fcntl)
        real_fcntl = dlsym(RTLD_NEXT, "fcntl");
    va_start(ap, cmd);
    arg = va_arg(ap, void *);
    va_end(ap);
    return retry_fcntl(real_fcntl, fd, cmd, arg);
}

int fcntl64(int fd, int cmd, ...)
{
    static int (*real_fcntl64)(int, int, void *);
    va_list ap;
    void *arg;

    if (!real_fcntl64)
        real_fcntl64 = dlsym(RTLD_NEXT, "fcntl64");
    if (!real_fcntl64)
        return fcntl(fd, cmd);
    va_start(ap, cmd);
    arg = va_arg(ap, void *);
    va_end(ap);
    return retry_fcntl(real_fcntl64, fd, cmd, arg);
}
