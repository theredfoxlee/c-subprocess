#include "subprocess_attr.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// -- Utilities

static void _subprocessattr_free_str_array(const char **array);
static const char ** _subprocessattr_copy_str_array(const char **array);
static void _subprocessattr_setstrarray(const char ***dst, const char **src);
static void _subprocessattr_setstr(const char **dst, const char *src);

// -- Core

struct subprocessattr {
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

subprocessattr_t *subprocessattr_init(void) {
    subprocessattr_t *attr = (subprocessattr_t *) malloc(sizeof(subprocessattr_t));

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

void subprocessattr_destroy(subprocessattr_t *attr) {
    if (NULL == attr) return;

    _subprocessattr_free_str_array(attr->args);
    _subprocessattr_free_str_array(attr->envs);

    free((char *) attr->envs);
    free((char *) attr->stdout_path);
    free((char *) attr->stderr_path);
    free((char *) attr->stdin_path);
    free((char *) attr->jail_path);
    free((char *) attr->cwd_path);

    free((char *) attr);
}

subprocessattr_t * subprocessattr_copy(const subprocessattr_t *attr) {
    subprocessattr_t *copy = subprocessattr_init();

    subprocessattr_setargs(copy, subprocessattr_getargs(attr));
    subprocessattr_setenvs(copy, subprocessattr_getenvs(attr));
    subprocessattr_setstdoutpath(copy, subprocessattr_getstdoutpath(attr));
    subprocessattr_setstderrpath(copy, subprocessattr_getstderrpath(attr));
    subprocessattr_setstdinpath(copy, subprocessattr_getstdinpath(attr));
    subprocessattr_setjailpath(copy, subprocessattr_getjailpath(attr));
    subprocessattr_setcwdpath(copy, subprocessattr_getcwdpath(attr));
    subprocessattr_setisolation(copy, subprocessattr_getisolation(attr));
    subprocessattr_setcustominit(copy, subprocessattr_getcustominit(attr));

    return copy;
}

void
subprocessattr_setargs(subprocessattr_t *attr, const char **args) {
    _subprocessattr_setstrarray(&attr->args, args);
}

void
subprocessattr_setenvs(subprocessattr_t *attr, const char **envs) {
    _subprocessattr_setstrarray(&attr->envs, envs);
}

void
subprocessattr_setstdoutpath(subprocessattr_t *attr, const char *stdout_path) {
    _subprocessattr_setstr(&attr->stdout_path, stdout_path);
}

void
subprocessattr_setstderrpath(subprocessattr_t *attr, const char *stderr_path) {
    _subprocessattr_setstr(&attr->stderr_path, stderr_path);
}

void
subprocessattr_setstdinpath(subprocessattr_t *attr, const char *stdin_path) {
    _subprocessattr_setstr(&attr->stdin_path, stdin_path);
}

void
subprocessattr_setjailpath(subprocessattr_t *attr, const char *jail_path) {
    _subprocessattr_setstr(&attr->jail_path, jail_path);
}

void
subprocessattr_setcwdpath(subprocessattr_t *attr, const char *cwd_path) {
    _subprocessattr_setstr(&attr->cwd_path, cwd_path);
}

void
subprocessattr_setisolation(subprocessattr_t *attr, bool isolation) {
    attr->isolation = isolation;
}

void
subprocessattr_setcustominit(subprocessattr_t *attr, custom_init_t custom_init) {
    attr->custom_init = custom_init;
}

const char **
subprocessattr_getargs(const subprocessattr_t *attr) {
    return attr->args;
}

const char **
subprocessattr_getenvs(const subprocessattr_t *attr) {
    return attr->envs;
}

const char *
subprocessattr_getstdoutpath(const subprocessattr_t *attr) {
    return attr->stdout_path;
}

const char *
subprocessattr_getstderrpath(const subprocessattr_t *attr) {
    return attr->stderr_path;
}

const char *
subprocessattr_getstdinpath(const subprocessattr_t *attr) {
    return attr->stdin_path;
}

const char *
subprocessattr_getjailpath(const subprocessattr_t *attr) {
    return attr->jail_path;
}

const char *
subprocessattr_getcwdpath(const subprocessattr_t *attr) {
    return attr->cwd_path;
}

bool
subprocessattr_getisolation(const subprocessattr_t *attr) {
    return attr->isolation;
}

custom_init_t
subprocessattr_getcustominit(const subprocessattr_t *attr) {
    return attr->custom_init;
}

// ---

static void
_subprocessattr_free_str_array(const char **array) {
    if (NULL == array) return;

    for (size_t i = 0; NULL != array[i]; ++i) {
        free((char *) array[i]);
    }

    free((char *) array);
}

static const char **
_subprocessattr_copy_str_array(const char **array) {
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
_subprocessattr_setstrarray(const char ***dst, const char **src) {
    if (NULL != *dst) {
        _subprocessattr_free_str_array(*dst);
    }
    *dst = src ? _subprocessattr_copy_str_array(src) : NULL;
}

static void
_subprocessattr_setstr(const char **dst, const char *src) {
    if (NULL != *dst) {
        free((char *) *dst);
    }
    *dst = src ? strdup(src) : NULL;
}

