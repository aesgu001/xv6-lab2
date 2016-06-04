#include "sema.h"
#include "user.h"

int MissionaryCount = 0, CannibalCount = 0;

struct Semaphore boat, cross;
struct Semaphore mutex;

void RowBoat(void);
void MissionaryArrives(void *);
void CannibalArrives(void *);

int main(void) {
	sem_init(&mutex);
	sem_signal(&mutex);

	sem_init(&boat);
	sem_signal(&boat);
	sem_signal(&boat);
	sem_signal(&boat);

	sem_init(&cross);

	if ((thread_create(MissionaryArrives,(void *)0)) == 0) {
		printf(1,"thread create failed!\n");
		exit();
	}
	if ((thread_create(CannibalArrives,(void *)0)) == 0) {
		printf(1,"thread create failed!\n");
		exit();
	}
	if ((thread_create(MissionaryArrives,(void *)0)) == 0) {
		printf(1,"thread create failed!\n");
		exit();
	}
	while (wait()>=0);
	exit();
}

void RowBoat(void) {
	printf(1,"Crossing river...\n");
	printf(1,"Missionaries: %d\n",MissionaryCount);
	printf(1,"Cannibals: %d\n",CannibalCount);
	if (MissionaryCount == 1 && CannibalCount == 2)
		printf(1,"Missionary eaten!\n\n");
	else
		printf(1,"Done!\n\n");
	MissionaryCount = CannibalCount = 0;
}

void MissionaryArrives(void *arg_ptr) {
	while (1) {
		sem_acquire(&boat);
		sem_acquire(&mutex);
		MissionaryCount++;
		if (MissionaryCount == 1 && CannibalCount == 2) {
			sem_signal(&mutex);
			sem_signal(&cross);
			sem_signal(&boat);
			sem_acquire(&cross);
			sem_acquire(&mutex);
			if (MissionaryCount == 0) {
				sem_signal(&mutex);
				sem_signal(&boat);
				texit();
			}
		}
		else if (MissionaryCount+CannibalCount == 3) {
			RowBoat();
			sem_signal(&mutex);
			sem_signal(&cross);
			sem_signal(&cross);
			sem_signal(&boat);
			texit();
		}
		else {
			sem_signal(&mutex);
			sem_acquire(&cross);
			sem_acquire(&mutex);
			if (MissionaryCount == 0) {
				sem_signal(&mutex);
				sem_signal(&boat);
				texit();
			}
		}

		MissionaryCount--;
		sem_signal(&mutex);
	}
}

void CannibalArrives(void *arg_ptr) {
	while (1) {
		sem_acquire(&boat);
		sem_acquire(&mutex);
		CannibalCount++;
		if (MissionaryCount == 1 && CannibalCount == 2) {
			sem_signal(&mutex);
			sem_signal(&cross);
			sem_signal(&boat);
			sem_acquire(&cross);
			sem_acquire(&mutex);
			if (CannibalCount == 0) {
				sem_signal(&mutex);
				sem_signal(&boat);
				texit();
			}
		}
		else if (MissionaryCount+CannibalCount == 3) {
			RowBoat();
			sem_signal(&mutex);
			sem_signal(&cross);
			sem_signal(&cross);
			sem_signal(&boat);
			texit();
		}
		else {
			sem_signal(&mutex);
			sem_acquire(&cross);
			sem_acquire(&mutex);
			if (CannibalCount == 0) {
				sem_signal(&mutex);
				sem_signal(&boat);
				texit();
			}
		}

		CannibalCount--;
		sem_signal(&mutex);
	}
}
