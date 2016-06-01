#include "sema.h"
#include "user.h"

int water_molecules, i, j, i_max, j_max;
struct Semaphore h, o;
lock_t mutex;

void hReady(void *arg_ptr);

void oReady(void *arg_ptr);

int main() {
	sem_init(&h);
	sem_init(&o);
	lock_init(&mutex);
	water_molecules = i = j = 0;
	j_max = random(20);
	i_max = j_max * 2;
	printf(1,"Creating water molecules...\n");
	void *tid;
	while (i++ < i_max) {
		tid = thread_create(hReady,(void *)0);
		if (tid == 0) {
			printf(1,"thread_create() failed!\n");
			exit();
		}
	}
	while (j++ < j_max) {
		tid = thread_create(oReady,(void *)0);
		if (tid == 0) {
			printf(1,"thread_create() failed!\n");
			exit();
		}
	}
	while(wait()>=0);
	printf(1,"Total created: %d\n",water_molecules);
	exit();
}

void hReady(void *arg_ptr) {
	sem_signal(&h);
	sem_acquire(&o);
	texit();
}

void oReady(void *arg_ptr) {
	sem_acquire(&h);
	sem_acquire(&h);
	sem_signal(&o);
	sem_signal(&o);
	lock_acquire(&mutex);
	water_molecules++;
	lock_release(&mutex);
	texit();
}
