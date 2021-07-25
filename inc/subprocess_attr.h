#pragma once

#include <stdbool.h>

typedef struct subprocess_attr subprocess_attr_t;
typedef int (*custom_init_t)(const subprocess_attr_t *);

subprocess_attr_t *subprocess_attr_init(void);
void subprocess_attr_destroy(subprocess_attr_t *attr);

subprocess_attr_t * subprocess_attr_copy(const subprocess_attr_t *attr);

void subprocess_attr_setargs(subprocess_attr_t *attr, const char **args);
void subprocess_attr_setenvs(subprocess_attr_t *attr, const char **envs);
void subprocess_attr_setstdoutpath(subprocess_attr_t *attr, const char *stdout_path);
void subprocess_attr_setstderrpath(subprocess_attr_t *attr, const char *stderr_path);
void subprocess_attr_setstdinpath(subprocess_attr_t *attr, const char *stdin_path);
void subprocess_attr_setjailpath(subprocess_attr_t *attr, const char *jail_path);
void subprocess_attr_setcwdpath(subprocess_attr_t *attr, const char *cwd_path);
void subprocess_attr_setisolation(subprocess_attr_t *attr, bool isolation);
void subprocess_attr_setcustominit(subprocess_attr_t *attr,custom_init_t custom_init);

const char **subprocess_attr_getargs(const subprocess_attr_t *attr);
const char **subprocess_attr_getenvs(const subprocess_attr_t *attr);
const char *subprocess_attr_getstdoutpath(const subprocess_attr_t *attr);
const char *subprocess_attr_getstderrpath(const subprocess_attr_t *attr);
const char *subprocess_attr_getstdinpath(const subprocess_attr_t *attr);
const char *subprocess_attr_getjailpath(const subprocess_attr_t *attr);
const char *subprocess_attr_getcwdpath(const subprocess_attr_t *attr);
bool subprocess_attr_getisolation(const subprocess_attr_t *attr);
custom_init_t subprocess_attr_getcustominit(const subprocess_attr_t *attr);
