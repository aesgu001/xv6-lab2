#include "types.h"
#include "user.h"
#include "queue.h"

int main(){
    struct queue *q = malloc(sizeof(struct queue));
    int i;
    init_q(q);
    for(i=0;i<10;i++){
        add_q(q,i);
		add_front_q(q,9-i);
    }
    for(;!empty_q(q);){
        printf(1,"pop %d\n",pop_q(q));
    }
    exit();
}
