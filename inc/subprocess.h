typedef struct executor subprocess_t;
typedef struct subprocess_attr subprocess_attr_t;

subprocess_t *subprocess_start(const subprocess_attr_t *attr);
void subprocess_destroy(subprocess_t *executor);

//bool subprocess_wait(subprocess_t *executor, useconds_t timeout);

int subprocess_getpid(const subprocess_t *executor);
int subprocess_getstatus(const subprocess_t *executor);
