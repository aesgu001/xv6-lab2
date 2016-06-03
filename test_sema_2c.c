#include "sema.h"
#include "user.h"

struct Semaphore tree, climb, dominant_active;
struct Semaphore mutex1, mutex2, mutex3, mutex4;
int moving_up, moving_down, count;
int dominant_count, dominant_ready;

void monkey_help(void);
void monkey(void *arg_ptr);
void dominant_monkey(void *arg_ptr);

int main(void) {
	moving_up = moving_down = 0;
	count = dominant_count = 0;
	dominant_ready = 0;
	
	// moving up
	sem_init(&mutex1);
	sem_signal(&mutex1);
	
	// moving down
	sem_init(&mutex2);
	sem_signal(&mutex2);

	// count
	sem_init(&mutex3);
	sem_signal(&mutex3);

	// dominant_count & dominant_ready
	sem_init(&mutex4);
	sem_signal(&mutex4);

	sem_init(&tree);
	sem_signal(&tree);
	sem_signal(&tree);
	sem_signal(&tree);

	sem_init(&climb);
	sem_signal(&climb);

	sem_init(&dominant_active);

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
	while(wait()>=0);
	printf(1,"All monkeys have coconuts.\n");
	exit();
}

void monkey_help(void) {
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

	sem_acquire(&mutex3);
	count++;
	printf(1,"Number of monkeys: %d\n",count);
	sem_signal(&mutex3);
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
	sem_signal(&mutex3);
	printf(1,"Done!\n");

	sem_signal(&tree);
}

void monkey(void *arg_ptr) {
	sem_acquire(&mutex4);
	if (dominant_ready) {
		dominant_count++;
		sem_signal(&mutex4);
		printf(1,"Waiting...\n");
		sem_acquire(&dominant_active);
		printf(1,"Done waiting!\n");
	}
	else
		sem_signal(&mutex4);
	monkey_help();
	texit();
}

void dominant_monkey(void *arg_ptr) {
	sem_acquire(&mutex4);
	dominant_ready = 1;
	sem_signal(&mutex4);
	printf(1,"Dominant Monkey ready!\n");
	
	monkey_help();
	printf(1,"Dominant Monkey done!\n");

	sem_acquire(&mutex4);
	dominant_ready = 0;
	while (dominant_count > 0) {
		sem_signal(&dominant_active);
		dominant_count = (dominant_count==0)? 0 : dominant_count - 1;
	}
	sem_signal(&mutex4);

	texit();
}
