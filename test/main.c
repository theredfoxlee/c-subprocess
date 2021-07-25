#include "subprocess_attr.h"

#include <stddef.h>

int
main() {
    // PREPARE ARGS
    executor_attr_t *attr = executor_attr_init();
    const char *args[] = {"/bin/ls", "/home", NULL};
    executor_attr_setargs(attr, args);
    executor_attr_setstdoutpath(attr, "/tmp/test.stdout");
    executor_attr_setstderrpath(attr, "/tmp/test.stderr");

    executor_attr_setisolation(attr, 0);

    // executor_t *executor = executor_start(attr);

    // fprintf(stderr, "%d\n", executor_getpid(executor));
    // fprintf(stderr, "%s\n", strerror(errno));

    // int status;
    // int ret = waitpid(executor_getpid(executor), &status, 0);

    // fprintf(stderr, "%d\n", ret);
    // fprintf(stderr, "%d\n", status);

    // fprintf(stderr, "%d\n", WIFEXITED(status));
    // fprintf(stderr, "%d\n", WIFSIGNALED(status));
    // fprintf(stderr, "%d\n", WEXITSTATUS(status));

    // if (-1 == executor_getpid(executor)) {
    //  return ret;
    // }

    // // WAIT ON CHILD'S END
    // if (!executor_wait(executor, 0)) {
    //  return -1;
    // }

    // // GET CHILD'S STATUS
    // const int status = executor_getstatus(executor);

    // // CLEANUP
    executor_attr_destroy(attr);
    return 0;
}

