#include "types.h"
#include "user.h"

void thread_print(void *);
void thread_yield(void *);

int main(void) {

	if (thread_create(thread_yield,(void *)0) == 0) {
		printf(1,"thread create fail!\n");
		exit();
	}
	if (thread_create(thread_print,(void *)0) == 0) {
		printf(1,"thread create fail!\n");
		exit();
	}
	if (thread_create(thread_print,(void *)0) == 0) {
		printf(1,"thread create fail!\n");
		exit();
	}
	while (wait()>=0);
	exit();
}

void thread_print(void *arg_ptr) {
	printf(1,"Printing thread: pid %d\n",getpid());
	texit();
}

void thread_yield(void *arg_ptr) {
	tyield();
	printf(1,"Yielding thread: pid %d\n",getpid());
	texit();
}
