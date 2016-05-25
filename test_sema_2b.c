#include "sema.h"
#include "user.h"

struct Semaphore mutex1, mutex2, tree, climb;
int moving_up = 0, moving_down = 0;

void monkey(void *arg_ptr);

int main() {
	sem_init(&mutex1);
	sem_signal(&mutex1);

	sem_init(&mutex2);
	sem_signal(&mutex2);

	sem_init(&tree);
	sem_signal(&tree);
	sem_signal(&tree);
	sem_signal(&tree);

	sem_init(&climb);
	sem_signal(&climb);

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

	sem_acquire(&mutex1);
	moving_up++;
	if (moving_up == 1)
		sem_acquire(&climb);
	sem_signal(&mutex1);
	printf(1,"Climbing up...\n");

	sem_acquire(&mutex1);
	moving_up--;
	if (moving_up == 0)
		sem_signal(&climb);
	sem_signal(&mutex1);
	printf(1,"Grabbing coconut...\n");

	sem_acquire(&mutex2);
	moving_down++;
	if (moving_down == 1)
		sem_acquire(&climb);
	sem_signal(&mutex2);
	printf(1,"Climbing down...\n");

	sem_acquire(&mutex2);
	moving_down--;
	if (moving_down == 0)
		sem_signal(&climb);
	sem_signal(&mutex2);
	printf(1,"Done!\n");

	sem_signal(&tree);
	texit();
}
