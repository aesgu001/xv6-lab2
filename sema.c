#include "sema.h"
#include "user.h"

void sem_init(struct Semaphore *s) {
	s->count = 0;
	lock_init(&s->lock);
	init_q(&s->q);
}

void sem_acquire(struct Semaphore *s) {
	lock_acquire(&s->lock);
	if (s->count == 0) {
		add_q(&s->q,getpid());
		lock_release(&s->lock);
		tsleep();
	}
	else {
		s->count--;
		lock_release(&s->lock);
	}
}

void sem_acquire_front(struct Semaphore *s) {
	lock_acquire(&s->lock);
	if (s->count == 0) {
		add_front_q(&s->q,getpid());
		lock_release(&s->lock);
		tsleep();
	}
	else {
		s->count--;
		lock_release(&s->lock);
	}
}

void sem_signal(struct Semaphore *s) {
	lock_acquire(&s->lock);
	if (s->count > 0) {
		s->count++;
		lock_release(&s->lock);
	}
	else {
		if (empty_q(&s->q) == 1)
			s->count++;
		else
			twakeup(pop_q(&s->q));
		lock_release(&s->lock);
	}
}
