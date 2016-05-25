#include "sema.h"
#include "user.h"

struct Semaphore tree;

void monkey(void *arg_ptr);

int main() {
	sem_init(&tree);
	sem_signal(&tree);
	sem_signal(&tree);
	sem_signal(&tree);
	void *tid = thread_create(monkey,(void *)0);
	if (tid == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	tid = thread_create(monkey,(void *)0);
	if (tid == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	tid = thread_create(monkey,(void *)0);
	if (tid == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	tid = thread_create(monkey,(void *)0);
	if (tid == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	while(wait()>=0);
	printf(1,"All monkeys have coconuts.\n");
	exit();
}

void monkey(void *arg_ptr) {
	sem_acquire(&tree);
	printf(1,"Grabbing coconut...\n");
	sem_signal(&tree);
	texit();
}
