#include "subprocess_attr.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// -- Utilities

static void _executor_attr_free_str_array(const char **array);
static const char ** _executor_attr_copy_str_array(const char **array);
static void _executor_attr_setstrarray(const char ***dst, const char **src);
static void _executor_attr_setstr(const char **dst, const char *src);

// -- Core

struct executor_attr {
    const char **args;

    const char **envs;

    const char *stdout_path;
    const char *stderr_path;
    const char *stdin_path;

    const char *jail_path;
    const char *cwd_path;

    bool isolation;

    custom_init_t custom_init;
};

executor_attr_t *executor_attr_init(void) {
    executor_attr_t *attr = (executor_attr_t *) malloc(sizeof(executor_attr_t));

    attr->args = NULL;
    attr->envs = NULL;
    attr->stdout_path = NULL;
    attr->stderr_path = NULL;
    attr->stdin_path = NULL;
    attr->jail_path = NULL;
    attr->cwd_path = NULL;
    attr->isolation = true;
    attr->custom_init = NULL;

    return attr;
}

void executor_attr_destroy(executor_attr_t *attr) {
    if (NULL == attr) return;

    _executor_attr_free_str_array(attr->args);
    _executor_attr_free_str_array(attr->envs);

    free((char *) attr->envs);
    free((char *) attr->stdout_path);
    free((char *) attr->stderr_path);
    free((char *) attr->stdin_path);
    free((char *) attr->jail_path);
    free((char *) attr->cwd_path);

    free((char *) attr);
}

executor_attr_t * executor_attr_copy(const executor_attr_t *attr) {
    executor_attr_t *copy = executor_attr_init();

    executor_attr_setargs(copy, executor_attr_getargs(attr));
    executor_attr_setenvs(copy, executor_attr_getenvs(attr));
    executor_attr_setstdoutpath(copy, executor_attr_getstdoutpath(attr));
    executor_attr_setstderrpath(copy, executor_attr_getstderrpath(attr));
    executor_attr_setstdinpath(copy, executor_attr_getstdinpath(attr));
    executor_attr_setjailpath(copy, executor_attr_getjailpath(attr));
    executor_attr_setcwdpath(copy, executor_attr_getcwdpath(attr));
    executor_attr_setisolation(copy, executor_attr_getisolation(attr));
    executor_attr_setcustominit(copy, executor_attr_getcustominit(attr));

    return copy;
}

void
executor_attr_setargs(executor_attr_t *attr, const char **args) {
    _executor_attr_setstrarray(&attr->args, args);
}

void
executor_attr_setenvs(executor_attr_t *attr, const char **envs) {
    _executor_attr_setstrarray(&attr->envs, envs);
}

void
executor_attr_setstdoutpath(executor_attr_t *attr, const char *stdout_path) {
    _executor_attr_setstr(&attr->stdout_path, stdout_path);
}

void
executor_attr_setstderrpath(executor_attr_t *attr, const char *stderr_path) {
    _executor_attr_setstr(&attr->stderr_path, stderr_path);
}

void
executor_attr_setstdinpath(executor_attr_t *attr, const char *stdin_path) {
    _executor_attr_setstr(&attr->stdin_path, stdin_path);
}

void
executor_attr_setjailpath(executor_attr_t *attr, const char *jail_path) {
    _executor_attr_setstr(&attr->jail_path, jail_path);
}

void
executor_attr_setcwdpath(executor_attr_t *attr, const char *cwd_path) {
    _executor_attr_setstr(&attr->cwd_path, cwd_path);
}

void
executor_attr_setisolation(executor_attr_t *attr, bool isolation) {
    attr->isolation = isolation;
}

void
executor_attr_setcustominit(executor_attr_t *attr, custom_init_t custom_init) {
    attr->custom_init = custom_init;
}

const char **
executor_attr_getargs(const executor_attr_t *attr) {
    return attr->args;
}

const char **
executor_attr_getenvs(const executor_attr_t *attr) {
    return attr->envs;
}

const char *
executor_attr_getstdoutpath(const executor_attr_t *attr) {
    return attr->stdout_path;
}

const char *
executor_attr_getstderrpath(const executor_attr_t *attr) {
    return attr->stderr_path;
}

const char *
executor_attr_getstdinpath(const executor_attr_t *attr) {
    return attr->stdin_path;
}

const char *
executor_attr_getjailpath(const executor_attr_t *attr) {
    return attr->jail_path;
}

const char *
executor_attr_getcwdpath(const executor_attr_t *attr) {
    return attr->cwd_path;
}

bool
executor_attr_getisolation(const executor_attr_t *attr) {
    return attr->isolation;
}

custom_init_t
executor_attr_getcustominit(const executor_attr_t *attr) {
    return attr->custom_init;
}

// ---

static void
_executor_attr_free_str_array(const char **array) {
    if (NULL == array) return;

    for (size_t i = 0; NULL != array[i]; ++i) {
        free((char *) array[i]);
    }

    free((char *) array);
}

static const char **
_executor_attr_copy_str_array(const char **array) {
    if (NULL == array) return NULL;

    size_t i;

    for (i = 0; NULL != array[i]; ++i);

    const size_t array_len = i;

    const char **copy = (const char **) malloc(sizeof(const char *) * array_len + 1);

    for (i = 0; i < array_len; ++i) {
        copy[i] = strdup(array[i]);
    }

    copy[array_len] = NULL;

    return copy;
}

static void 
_executor_attr_setstrarray(const char ***dst, const char **src) {
    if (NULL != *dst) {
        _executor_attr_free_str_array(*dst);
    }
    *dst = src ? _executor_attr_copy_str_array(src) : NULL;
}

static void
_executor_attr_setstr(const char **dst, const char *src) {
    if (NULL != *dst) {
        free((char *) *dst);
    }
    *dst = src ? strdup(src) : NULL;
}

