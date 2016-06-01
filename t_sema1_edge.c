#include "sema.h"
#include "user.h"

int water_molecules, i, j;
struct Semaphore h, o;
lock_t mutex;

void hReady(void *arg_ptr);

void oReady(void *arg_ptr);

int main() {
	sem_init(&h);
	sem_init(&o);
	lock_init(&mutex);
	water_molecules = i = j = 0;
	printf(1,"Creating water molecules...\n");
	void *tid;
	while (i++ < 40) {
		tid = thread_create(hReady,(void *)0);
		if (tid == 0) {
			printf(1,"thread_create() failed!\n");
			exit();
		}
	}
	while (j++ < 20) {
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
