typedef struct executor subprocesst;
typedef struct subprocessattr subprocessattr_t;

subprocesst *subprocessstart(const subprocessattr_t *attr);
void subprocessdestroy(subprocesst *executor);

//bool subprocesswait(subprocesst *executor, useconds_t timeout);

int subprocessgetpid(const subprocesst *executor);
int subprocessgetstatus(const subprocesst *executor);
