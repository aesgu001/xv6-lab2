#include "sema.h"
#include "user.h"

struct Semaphore tree, mutex;
int count, i;

void monkey(void *arg_ptr);

int main() {
	count = i = 0;

	sem_init(&tree);
	sem_signal(&tree);
	sem_signal(&tree);
	sem_signal(&tree);

	sem_init(&mutex);
	sem_signal(&mutex);

	while (i++ < 10) {
		if (thread_create(monkey,(void *)0) == 0) {
			printf(1,"thread_create() failed!\n");
			exit();
		}
	}
	while(wait()>=0);
	printf(1,"All monkeys have coconuts.\n");
	exit();
}

void monkey(void *arg_ptr) {
	sem_acquire(&tree);

	sem_acquire(&mutex);
	count++;
	printf(1,"Number of monkeys: %d\n",count);
	sem_signal(&mutex);

	printf(1,"Grabbing coconut...\n");

	sem_acquire(&mutex);
	count--;
	sem_signal(&mutex);

	sem_signal(&tree);
	texit();
}
