#include "subprocess_.h"
#include "subprocess__attr.h"

// #define _GNU_SOURCE
// #include <linux/sched.h>        /* or #include <sched.h> */

#define _GNU_SOURCE
#include <unistd.h>
#include <linux/sched.h>
#include <sched.h>

#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

struct executor {
    pid_t pid;
    int status;
};

int
_subprocess_child_replace_fd_with_file(int old_fd, const char *path, int oflag) {
    if (NULL == path) return 0;

    int fd = open(path, oflag, 0666);

    if (-1 == fd) {
        return -1;
    }

    return -1 == dup2(fd, old_fd);
}

static int
_subprocess_child_run_custom_init(const subprocess_attr_t *attr, custom_init_t custom_init) {
    if (NULL == custom_init) return 0;
    return custom_init(attr);
}

static int
_subprocess_child_replace_stdout_with_file(const char *stdout_path) {
    return _subprocess_child_replace_fd_with_file(
        STDOUT_FILENO, stdout_path, O_WRONLY | O_CREAT | O_TRUNC);
}

static int
_subprocess_child_replace_stderr_with_file(const char *stderr_path) {
    return _subprocess_child_replace_fd_with_file(
        STDERR_FILENO, stderr_path, O_WRONLY | O_CREAT | O_TRUNC);
}


static int
_subprocess_child_replace_stdin_with_file(const char *stdin_path) {
    return _subprocess_child_replace_fd_with_file(
        STDIN_FILENO, stdin_path, O_RDONLY);
}

static int
_subprocess_child_chroot(const char *jail_path) {
    if (NULL == jail_path) return 0;

    int ret = 0;

    if ((ret = chroot(jail_path))) {
        return ret;
    }

    if ((ret = chdir("/"))) {
        return ret;
    }

    // if ((ret = mount("none", "/sys", "sysfs", 0, ""))) {
    //     return ret;
    // }

    // if ((ret = mount("none", "/proc", "proc", 0, ""))) {
    //     return ret;
    // }

    // if ((ret = mount("none", "/dev", "devtmpfs", 0, ""))) {
    //     return ret;
    // }

    // if ((ret = mkdir("/tmp", 0666))) {
    //     return ret;
    // }

    return ret;
}

static int
_subprocess_child_cd_cwd(const char *cwd) {
    if (NULL == cwd) cwd = "/";
    return chdir(cwd); 
}

static int
_subprocess_child_exec(const char **args, const char **envs) {
    return execvpe(args[0], (char * const*) args, (char * const*) envs);
}

#include <stdio.h>

static int
_main(void *data) {
    subprocess_attr_t *attr = (subprocess_attr_t *) data;

    int ret = 0;

    if ((ret = _subprocess_child_run_custom_init(attr, subprocess_attr_getcustominit(attr)))) {
        return ret;
    }

    if ((ret = _subprocess_child_replace_stdout_with_file(subprocess_attr_getstdoutpath(attr)))) {
        return ret;
    }

    if ((ret = _subprocess_child_replace_stderr_with_file(subprocess_attr_getstderrpath(attr)))) {
        return ret;
    }

    if ((ret = _subprocess_child_replace_stdin_with_file(subprocess_attr_getstdinpath(attr)))) {
        return ret;
    }

    if ((ret = _subprocess_child_chroot(subprocess_attr_getjailpath(attr)))) {
        return ret;
    }

    if ((ret = _subprocess_child_cd_cwd(subprocess_attr_getcwdpath(attr)))) {
        return ret;
    }

    return _subprocess_child_exec(subprocess_attr_getargs(attr), subprocess_attr_getenvs(attr));
}

subprocess_t *subprocess_start(const subprocess_attr_t *attr) {
    subprocess_t *executor = (subprocess_t *) malloc(sizeof(subprocess_t));

    long stack_size = sysconf(_SC_PAGESIZE);
    void *stack = (char *) alloca(stack_size) + stack_size;

    int flags = CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID | SIGCHLD;

    if (subprocess_attr_getisolation(attr)) {
        // pid namespace, ipc namespace, mount namespace
        flags |= CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWNS;
    }

    executor->pid = clone(_main, stack, flags, (void *)attr);
    executor->status = -1;

    return executor;
}

void subprocess_destroy(subprocess_t *executor) {
    // if (-1 != executor->pid && !subprocess_wait(executor, 0)) {
    //     kill(executor->pid, SIGKILL);
    // }
    free(executor);
}

// bool subprocess_wait(subprocess_t *executor, useconds_t timeout) {
//     return false;
// }

int subprocess_getpid(const subprocess_t *executor) {
    return executor->pid;
}

int subprocess_getstatus(const subprocess_t *executor) {
    return executor->status;
}

