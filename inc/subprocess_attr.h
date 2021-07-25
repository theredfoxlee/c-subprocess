#pragma once

#include <stdbool.h>

typedef struct executor_attr executor_attr_t;
typedef int (*custom_init_t)(const executor_attr_t *);

executor_attr_t *executor_attr_init(void);
void executor_attr_destroy(executor_attr_t *attr);

executor_attr_t * executor_attr_copy(const executor_attr_t *attr);

void executor_attr_setargs(executor_attr_t *attr, const char **args);
void executor_attr_setenvs(executor_attr_t *attr, const char **envs);
void executor_attr_setstdoutpath(executor_attr_t *attr, const char *stdout_path);
void executor_attr_setstderrpath(executor_attr_t *attr, const char *stderr_path);
void executor_attr_setstdinpath(executor_attr_t *attr, const char *stdin_path);
void executor_attr_setjailpath(executor_attr_t *attr, const char *jail_path);
void executor_attr_setcwdpath(executor_attr_t *attr, const char *cwd_path);
void executor_attr_setisolation(executor_attr_t *attr, bool isolation);
void executor_attr_setcustominit(executor_attr_t *attr,custom_init_t custom_init);

const char **executor_attr_getargs(const executor_attr_t *attr);
const char **executor_attr_getenvs(const executor_attr_t *attr);
const char *executor_attr_getstdoutpath(const executor_attr_t *attr);
const char *executor_attr_getstderrpath(const executor_attr_t *attr);
const char *executor_attr_getstdinpath(const executor_attr_t *attr);
const char *executor_attr_getjailpath(const executor_attr_t *attr);
const char *executor_attr_getcwdpath(const executor_attr_t *attr);
bool executor_attr_getisolation(const executor_attr_t *attr);
custom_init_t executor_attr_getcustominit(const executor_attr_t *attr);
