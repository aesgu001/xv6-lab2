#include "queue.h"
#include "types.h"

struct Semaphore {
	int count;
	lock_t lock;
	struct queue q;
};

void sem_init(struct Semaphore *);
void sem_acquire(struct Semaphore *);
void sem_signal(struct Semaphore *);
