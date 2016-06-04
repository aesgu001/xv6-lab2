#include "sema.h"
#include "user.h"

struct Semaphore tree, climb;
struct Semaphore mutex1, mutex2, mutex3;
int moving_up, moving_down, count;

void monkey(void *arg_ptr);
void dominant_monkey(void *arg_ptr);

int main(void) {
	moving_up = moving_down = 0;
	count = 0;
	
	// moving up
	sem_init(&mutex1);
	sem_signal(&mutex1);
	
	// moving down
	sem_init(&mutex2);
	sem_signal(&mutex2);

	// count
	sem_init(&mutex3);
	sem_signal(&mutex3);

	sem_init(&tree);
	sem_signal(&tree);
	sem_signal(&tree);
	sem_signal(&tree);

	sem_init(&climb);
	sem_signal(&climb);

	if (thread_create(monkey,(void *)0) == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	if (thread_create(monkey,(void *)0) == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	if (thread_create(monkey,(void *)0) == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	if (thread_create(dominant_monkey,(void *)0) == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	if (thread_create(monkey,(void *)0) == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	if (thread_create(monkey,(void *)0) == 0) {
		printf(1,"thread_create() failed!\n");
		exit();
	}
	while(wait()>=0);
	printf(1,"All monkeys have coconuts.\n");
	exit();
}

void monkey(void *arg_ptr) {
	sem_acquire(&tree);

	sem_acquire(&mutex3);
	count++;
	printf(1,"Number of monkeys: %d\n",count);
	sem_signal(&mutex3);

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

	sem_acquire(&mutex3);
	count--;
	printf(1,"Done!\n");
	sem_signal(&mutex3);
	
	sem_signal(&tree);
	texit();
}

void dominant_monkey(void *arg_ptr) {
	sem_acquire_front(&tree);

	sem_acquire_front(&mutex3);
	count++;
	printf(1,"Number of monkeys: %d\n",count);
	sem_signal(&mutex3);

	sem_acquire_front(&mutex1);
	moving_up++;
	if (moving_up == 1)
		sem_acquire_front(&climb);
	sem_signal(&mutex1);
	printf(1,"Dominant climbing up...\n");

	sem_acquire_front(&mutex1);
	moving_up--;
	if (moving_up == 0)
		sem_signal(&climb);
	sem_signal(&mutex1);

	printf(1,"Dominant grabbing coconut...\n");

	sem_acquire_front(&mutex2);
	moving_down++;
	if (moving_down == 1)
		sem_acquire_front(&climb);
	sem_signal(&mutex2);
	printf(1,"Dominant climbing down...\n");

	sem_acquire_front(&mutex2);
	moving_down--;
	if (moving_down == 0)
		sem_signal(&climb);
	sem_signal(&mutex2);

	sem_acquire_front(&mutex3);
	count--;
	printf(1,"Dominant done!\n");
	sem_signal(&mutex3);
	
	sem_signal(&tree);
	texit();
}
