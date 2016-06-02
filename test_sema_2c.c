#include "sema.h"
#include "user.h"

struct Semaphore mutex1, mutex2, tree, climb;
int moving_up = 0, moving_down = 0;
int dominant_ready, i;
struct queue dominant_q;

void monkey_help(void);
void monkey(void *arg_ptr);
void dominant_monkey(void *arg_ptr);

int main() {
	init_q(&dominant_q);
	dominant_ready = 0;
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

	i = 0;
	void *tid;
	while (i++ < 6) {
		tid = thread_create(monkey,(void *)0);
		if (tid == 0) {
			printf(1,"thread_create() failed!\n");
			exit();
		}
	}
	tid = thread_create(dominant_monkey,(void *)0);
	if (tid == 0) {
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
}

void monkey(void *arg_ptr) {
	if (dominant_ready) {
		add_q(&dominant_q,getpid());
		tsleep();
	}
	monkey_help();
	texit();
}

void dominant_monkey(void *arg_ptr) {
	printf(1,"\nDominant Monkey appears!\n");
	dominant_ready = 1;
	monkey_help();
	printf(1,"Dominant Monkey done!\n\n");
	while (!empty_q(&dominant_q))
		twakeup(pop_q(&dominant_q));
	dominant_ready = 0;
	texit();
}
