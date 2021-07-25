#include "subprocess.h"
#include "subprocess_attr.h"

#include <stddef.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int
main() {
    // PREPARE ARGS
    subprocess_attr_t *attr = subprocess_attr_init();
    const char *args[] = {"/bin/ls", "/home", NULL};
    subprocess_attr_setargs(attr, args);
    subprocess_attr_setstdoutpath(attr, "/tmp/test.stdout");
    subprocess_attr_setstderrpath(attr, "/tmp/test.stderr");

    subprocess_attr_setisolation(attr, 0);

    subprocess_t *executor = subprocess_start(attr);

    fprintf(stderr, "%d\n", subprocess_getpid(executor));
    fprintf(stderr, "%s\n", strerror(errno));

    int status;
    int ret = waitpid(subprocess_getpid(executor), &status, 0);

    fprintf(stderr, "%d\n", ret);
    fprintf(stderr, "%d\n", status);

    fprintf(stderr, "%d\n", WIFEXITED(status));
    fprintf(stderr, "%d\n", WIFSIGNALED(status));
    fprintf(stderr, "%d\n", WEXITSTATUS(status));

    //if (-1 == subprocess_getpid(executor)) {
    // return ret;
    //}

    // WAIT ON CHILD'S END
    //if (!subprocess_wait(executor, 0)) {
    // return -1;
    //}

    // GET CHILD'S STATUS
    //const int status = subprocess_getstatus(executor);

    // CLEANUP
    subprocess_attr_destroy(attr);
    subprocess_destroy(executor);
    return 0;
}

