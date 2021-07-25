#pragma once

#include <stdbool.h>

typedef struct subprocessattr subprocessattr_t;
typedef int (*custom_init_t)(const subprocessattr_t *);

subprocessattr_t *subprocessattr_init(void);
void subprocessattr_destroy(subprocessattr_t *attr);

subprocessattr_t * subprocessattr_copy(const subprocessattr_t *attr);

void subprocessattr_setargs(subprocessattr_t *attr, const char **args);
void subprocessattr_setenvs(subprocessattr_t *attr, const char **envs);
void subprocessattr_setstdoutpath(subprocessattr_t *attr, const char *stdout_path);
void subprocessattr_setstderrpath(subprocessattr_t *attr, const char *stderr_path);
void subprocessattr_setstdinpath(subprocessattr_t *attr, const char *stdin_path);
void subprocessattr_setjailpath(subprocessattr_t *attr, const char *jail_path);
void subprocessattr_setcwdpath(subprocessattr_t *attr, const char *cwd_path);
void subprocessattr_setisolation(subprocessattr_t *attr, bool isolation);
void subprocessattr_setcustominit(subprocessattr_t *attr,custom_init_t custom_init);

const char **subprocessattr_getargs(const subprocessattr_t *attr);
const char **subprocessattr_getenvs(const subprocessattr_t *attr);
const char *subprocessattr_getstdoutpath(const subprocessattr_t *attr);
const char *subprocessattr_getstderrpath(const subprocessattr_t *attr);
const char *subprocessattr_getstdinpath(const subprocessattr_t *attr);
const char *subprocessattr_getjailpath(const subprocessattr_t *attr);
const char *subprocessattr_getcwdpath(const subprocessattr_t *attr);
bool subprocessattr_getisolation(const subprocessattr_t *attr);
custom_init_t subprocessattr_getcustominit(const subprocessattr_t *attr);
