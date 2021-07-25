#include "subprocess__attr.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// -- Utilities

static void _subprocess_attr_free_str_array(const char **array);
static const char ** _subprocess_attr_copy_str_array(const char **array);
static void _subprocess_attr_setstrarray(const char ***dst, const char **src);
static void _subprocess_attr_setstr(const char **dst, const char *src);

// -- Core

struct subprocess_attr {
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

subprocess_attr_t *subprocess_attr_init(void) {
    subprocess_attr_t *attr = (subprocess_attr_t *) malloc(sizeof(subprocess_attr_t));

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

void subprocess_attr_destroy(subprocess_attr_t *attr) {
    if (NULL == attr) return;

    _subprocess_attr_free_str_array(attr->args);
    _subprocess_attr_free_str_array(attr->envs);

    free((char *) attr->envs);
    free((char *) attr->stdout_path);
    free((char *) attr->stderr_path);
    free((char *) attr->stdin_path);
    free((char *) attr->jail_path);
    free((char *) attr->cwd_path);

    free((char *) attr);
}

subprocess_attr_t * subprocess_attr_copy(const subprocess_attr_t *attr) {
    subprocess_attr_t *copy = subprocess_attr_init();

    subprocess_attr_setargs(copy, subprocess_attr_getargs(attr));
    subprocess_attr_setenvs(copy, subprocess_attr_getenvs(attr));
    subprocess_attr_setstdoutpath(copy, subprocess_attr_getstdoutpath(attr));
    subprocess_attr_setstderrpath(copy, subprocess_attr_getstderrpath(attr));
    subprocess_attr_setstdinpath(copy, subprocess_attr_getstdinpath(attr));
    subprocess_attr_setjailpath(copy, subprocess_attr_getjailpath(attr));
    subprocess_attr_setcwdpath(copy, subprocess_attr_getcwdpath(attr));
    subprocess_attr_setisolation(copy, subprocess_attr_getisolation(attr));
    subprocess_attr_setcustominit(copy, subprocess_attr_getcustominit(attr));

    return copy;
}

void
subprocess_attr_setargs(subprocess_attr_t *attr, const char **args) {
    _subprocess_attr_setstrarray(&attr->args, args);
}

void
subprocess_attr_setenvs(subprocess_attr_t *attr, const char **envs) {
    _subprocess_attr_setstrarray(&attr->envs, envs);
}

void
subprocess_attr_setstdoutpath(subprocess_attr_t *attr, const char *stdout_path) {
    _subprocess_attr_setstr(&attr->stdout_path, stdout_path);
}

void
subprocess_attr_setstderrpath(subprocess_attr_t *attr, const char *stderr_path) {
    _subprocess_attr_setstr(&attr->stderr_path, stderr_path);
}

void
subprocess_attr_setstdinpath(subprocess_attr_t *attr, const char *stdin_path) {
    _subprocess_attr_setstr(&attr->stdin_path, stdin_path);
}

void
subprocess_attr_setjailpath(subprocess_attr_t *attr, const char *jail_path) {
    _subprocess_attr_setstr(&attr->jail_path, jail_path);
}

void
subprocess_attr_setcwdpath(subprocess_attr_t *attr, const char *cwd_path) {
    _subprocess_attr_setstr(&attr->cwd_path, cwd_path);
}

void
subprocess_attr_setisolation(subprocess_attr_t *attr, bool isolation) {
    attr->isolation = isolation;
}

void
subprocess_attr_setcustominit(subprocess_attr_t *attr, custom_init_t custom_init) {
    attr->custom_init = custom_init;
}

const char **
subprocess_attr_getargs(const subprocess_attr_t *attr) {
    return attr->args;
}

const char **
subprocess_attr_getenvs(const subprocess_attr_t *attr) {
    return attr->envs;
}

const char *
subprocess_attr_getstdoutpath(const subprocess_attr_t *attr) {
    return attr->stdout_path;
}

const char *
subprocess_attr_getstderrpath(const subprocess_attr_t *attr) {
    return attr->stderr_path;
}

const char *
subprocess_attr_getstdinpath(const subprocess_attr_t *attr) {
    return attr->stdin_path;
}

const char *
subprocess_attr_getjailpath(const subprocess_attr_t *attr) {
    return attr->jail_path;
}

const char *
subprocess_attr_getcwdpath(const subprocess_attr_t *attr) {
    return attr->cwd_path;
}

bool
subprocess_attr_getisolation(const subprocess_attr_t *attr) {
    return attr->isolation;
}

custom_init_t
subprocess_attr_getcustominit(const subprocess_attr_t *attr) {
    return attr->custom_init;
}

// ---

static void
_subprocess_attr_free_str_array(const char **array) {
    if (NULL == array) return;

    for (size_t i = 0; NULL != array[i]; ++i) {
        free((char *) array[i]);
    }

    free((char *) array);
}

static const char **
_subprocess_attr_copy_str_array(const char **array) {
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
_subprocess_attr_setstrarray(const char ***dst, const char **src) {
    if (NULL != *dst) {
        _subprocess_attr_free_str_array(*dst);
    }
    *dst = src ? _subprocess_attr_copy_str_array(src) : NULL;
}

static void
_subprocess_attr_setstr(const char **dst, const char *src) {
    if (NULL != *dst) {
        free((char *) *dst);
    }
    *dst = src ? strdup(src) : NULL;
}

