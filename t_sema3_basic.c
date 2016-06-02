#include "sema.h"
#include "user.h"

int MissionaryCount = 0, CannibalCount = 0;
int BoatReady;
struct Semaphore boat;
struct queue wait_q;
lock_t mutexM, mutexC, mutexR;

void RowBoat(void);
void MissionaryArrives(void *);
void CannibalArrives(void *);

int main(void) {
	init_q(&wait_q);
	lock_init(&mutexM);
	lock_init(&mutexC);
	lock_init(&mutexR);
	sem_init(&boat);
	sem_signal(&boat);
	sem_signal(&boat);
	sem_signal(&boat);
	BoatReady = 0;

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
	if (CannibalCount == 2 && MissionaryCount == 1)
		printf(1,"Missionary eaten!.. Fail!\n\n");
	else
		printf(1,"Done!\n\n");
//	lock_acquire(
}

void MissionaryArrives(void *arg_ptr) {
	sem_acquire(&boat);
	lock_acquire(&mutexM);
	MissionaryCount++;
	lock_release(&mutexM);

	lock_acquire(&mutexC);
	while (CannibalCount == 2) {
		lock_release(&mutexC);
//		add_q(&wait_q,getpid());
		sem_signal(&boat);
		lock_acquire(&mutexM);
		MissionaryCount--;
		lock_release(&mutexM);
		tsleep();

		sem_acquire(&boat);
		lock_acquire(&mutexM);
		MissionaryCount++;
		lock_release(&mutexM);
		lock_acquire(&mutexC);
	}
	lock_release(&mutexC);

	// RowBoat()

//	if (!empty_q(&wait_q))
//		twakeup(pop_q(&wait_q));
	sem_signal(&boat);
	lock_acquire(&mutexM);
	MissionaryCount--;
	lock_acquire(&mutexM);
	texit();
}

void CannibalArrives(void *arg_ptr) {
	sem_acquire(&boat);
	lock_acquire(&mutexC);
	CannibalCount++;
	lock_release(&mutexC);

	lock_acquire(&mutexM);
	while (MissionaryCount == 1) {
		lock_release(&mutexM);
//		add_q(&wait_q,getpid());
		sem_signal(&boat);
		lock_acquire(&mutexC);
		CannibalCount--;
		lock_release(&mutexC);
		tsleep();

		sem_acquire(&boat);
		lock_acquire(&mutexC);
		CannibalCount++;
		lock_release(&mutexC);
		lock_acquire(&mutexM);
	}
	lock_release(&mutexM);
	
	// RowBoat()

//	if (!empty_q(&wait_q))
//		twakeup(pop_q(&wait_q));
	sem_signal(&boat);
	lock_acquire(&mutexC);
	CannibalCount--;
	lock_release(&mutexC);
	texit();
}
