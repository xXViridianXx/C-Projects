#include "node.h"
#include "pqe.h"
#include "pq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//struct for pq entry
struct PQEntry {
    char *author;
    double distance;
};

//struct for pq
struct PriorityQueue { //PriorityQueue struct
    uint32_t tail;
    uint32_t head;
    uint32_t capacity;
    PQEntry **Q;
};

//allocates memory to pqentry
PQEntry *entry_create(char *author, double distance) {
    PQEntry *e = malloc(sizeof(PQEntry));
    //settin vars
    if (e) {
        e->author = author;
        e->distance = distance;
        return e;
    }
    free(e);
    e = NULL;
    return (PQEntry *) 0;
}

void entry_delete(PQEntry **e) {
    //deletes strdup if created
    if (*e && (*e)->author)
        free((*e)->author);
    free(*e);
    *e = NULL;
}

//allocate memory to pq
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q
        = (PriorityQueue *) malloc(sizeof(PriorityQueue)); //allocates memory to PriorityQueue q
    //sets  vars
    if (q) {
        q->tail = q->head = 0; //initialize
        q->capacity = capacity;
        //creates array of entries
        q->Q = (PQEntry **) calloc(q->capacity, sizeof(PQEntry)); //creates Node array
        if (q->Q) { // checking if item is non-zero
            return q;
        }
        free(q); //if null free and return 0
        q = NULL;
    }
    return (PriorityQueue *) 0;
}

//deletes pq
void pq_delete(PriorityQueue **q) {
    //deletes every entry in pq
    for (uint32_t i = 0; i < (*q)->capacity; i++) {
        entry_delete(&((*q)->Q[i]));
    }

    free((*q)->Q); //deletes Node array
    (*q)->Q = NULL;

    free(*q); //deletes priorityqueue
    *q = NULL;
}

//check if pq empty
bool pq_empty(PriorityQueue *q) { //checks is pq empty
    if (q) {

        return (pq_size(q) == 0); //empty if size == 0
    }
    return true;
}
//check if pq full
bool pq_full(PriorityQueue *q) { //checks if pq full

    if (q) {
        return (q->head) == q->capacity; //full if size == capacity
    }
    return true;
}
//get current size
uint32_t pq_size(PriorityQueue *q) { //returns current size of queue
    return q->head;
}
//using heapsort from asgn3
uint32_t min_child(PQEntry **Q, uint32_t first, uint32_t last) { /*finds largest child and returns*/
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && ((Q[right - 1])->distance < (Q[left - 1])->distance)) {
        return right;
    }
    return left;
}

void fix_heap(PQEntry **Q, uint32_t first, uint32_t last) { /*sorts array*/
    bool found = false;
    uint32_t mother = first;
    uint32_t great = min_child(Q, mother, last);
    uint32_t range = last / 2;

    while ((mother <= range) && (found == false)) {
        if ((Q[great - 1])->distance < (Q[mother - 1])->distance) { /*swap mother and min child*/
            PQEntry *t = Q[mother - 1];
            Q[mother - 1] = Q[great - 1];
            Q[great - 1] = t;
            mother = great;
            great = min_child(Q, mother, last);
        } else {
            found = true;
        }
    }
}
//debugging function
void entry_print(PQEntry *e) {
    printf("(author: %s, %f)\n", e->author, e->distance);
}
//enqueue then fix heap
bool enqueue(PriorityQueue *q, char *author, double distance) {
    if (q) {
        if (pq_full(q)) {
            return false;
        }
        //use strdup to copy over
        char *a = strdup(author);
        PQEntry *entry = entry_create(a, distance);
        q->Q[q->head] = entry; //head pointing to entry n
        q->head += 1;
        if (pq_size(q) != 1) {
            uint32_t range = pq_size(q) / 2;
            uint32_t first = 1;
            for (uint32_t father = range; father > first - 1; father--) {
                fix_heap(q->Q, father, pq_size(q));
            };
        }
        return true;
    }
    return false;
}
//pops first thing in array, swaps with last then swap and decrease size
bool dequeue(PriorityQueue *q, char **author, double *dist) {
    if (q) {
        if (pq_empty(q)) {
            return false;
        }

        PQEntry *temp = NULL;
        //setting pointers
        *author = (q->Q[q->tail])->author;
        *dist = (q->Q[q->tail])->distance;
        //swapping
        temp = q->Q[q->head - 1];
        q->Q[q->head - 1] = q->Q[q->tail];
        q->Q[q->tail] = temp;

        q->head -= 1;
        uint32_t range = pq_size(q) / 2;
        uint32_t first = 1;
        //fixing heap again
        for (uint32_t father = range; father > first - 1; father--) {
            fix_heap(q->Q, father, pq_size(q));
        }
    }
    return true;
}

void pq_print(PriorityQueue *q) { //debugging print statement
    for (uint32_t i = q->tail; i < q->head; i++) {
        printf("author: %s, distance: %f\n", (q->Q[i]->author), (q->Q[i])->distance);
    }
    printf("size: %d\n", pq_size(q));
}

/*int main(void) {
	PriorityQueue *pq = pq_create(8);
	PQEntry *s = entry_create(NULL, 0);
	enqueue(pq, "Aniket", 5);
	enqueue(pq, "Bob", 2);
	enqueue(pq, "Sarah", 7);
	enqueue(pq, "Avicii", 1);
	enqueue(pq, "Ned", 5);
	enqueue(pq, "Bruce", 3);
	enqueue(pq, "Barry", 1);
	printf("pq: \n");
	pq_print(pq);

	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	dequeue(pq, &(s->author), &(s->distance));
	printf("entry dequeued: ");
	entry_print(s);
	
	entry_delete(&s);
	pq_delete(&pq);
	return 0;
}*/
