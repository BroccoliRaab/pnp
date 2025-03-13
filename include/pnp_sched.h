#ifndef PNP_SCHED_H
#define PNP_SCHED_H

/*
 * Basic Threading model
 *
 * Both cores are trying to steal work from the queue.
 *
 * Tasks can put work at the front of the queue or the back of the queue.
 * Although they should only push to the front for task synchonization,
 *      ie, render pipeline, physics devices
 *
 * Tasks themselves should not spinlock(including hardware mutexes).
 *      Instead they should requeue themselves to the back of the queue.
 *
 * When pushing (back or front) to a full queue of a full queue:
 *      Peek the front and run it.
 *      Maybe Store Stack Frame? Feels like a premature optimization.
 *      Then atomically pop and push.
 */

#define PNP_SCHED_TASK_MAX (64)

typedef void (*task_f)(void *);

struct pnp_sched_task_s
{
    task_f function;
    void * parameter;
};

typedef struct pnp_sched_task_s pnp_sched_task_t;

/* Initialize the scheduler datastructure */
/* Prime the scheduler with an entry point */
void
pnp_sched_init(pnp_sched_task_t entry);

/* Consume and execute tasks from the queue */
/* This is the entry for core 1 and post-init sequence for core 0 */
void
pnp_sched_worker(void);

pnp_sched_task_t
pnp_pop_back(void);

pnp_sched_task_t
pnp_peek_back(void);

void
pnp_push_back(pnp_sched_task_t);

pnp_sched_task_t
pnp_pop_front(void);

pnp_sched_task_t
pnp_peek_front(void);

void
pnp_push_front(pnp_sched_task_t);

#endif /* PNP_SCHED_H */
