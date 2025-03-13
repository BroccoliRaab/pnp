#include <stdint.h>

#include "pnp_platform.h"
#include "pnp_sched.h"

#define NULL_TASK ((pnp_sched_task_t){0,0})

static uint32_t pnp_sched_front;
static uint32_t pnp_sched_sz;
static pnp_lock_t front_lock, back_lock;
static pnp_sched_task_t pnp_sched_task_deque[PNP_SCHED_TASK_MAX];

void
pnp_sched_worker(void)
{
    pnp_sched_task_t task;
    do {
        task = pnp_pop_front();
        if (task.function) task.function(task.parameter);
    } while(1);
    
}

void
pnp_sched_init(pnp_sched_task_t entry)
{
    front_lock = PNP_GET_LOCK();
    back_lock = PNP_GET_LOCK();
    if (!(back_lock && front_lock)) return; /*TODO: Panic*/
    pnp_sched_sz = 0;
    pnp_sched_front = 0;
    pnp_push_front(entry);
}

/* Deque Interface */
pnp_sched_task_t
pnp_pop_back(void)
{
    pnp_sched_task_t ret;
    uint32_t back;
    if (pnp_sched_sz == 0) return NULL_TASK; /*TODO: Error*/
    PNP_LOCK(back_lock);
    back = (pnp_sched_front + pnp_sched_sz - 1) % PNP_SCHED_TASK_MAX;
    pnp_sched_sz--;
    ret = pnp_sched_task_deque[back];
    PNP_UNLOCK(back_lock);
    return ret;
}

pnp_sched_task_t
pnp_peek_back(void)
{
    pnp_sched_task_t ret;
    uint32_t back;
    if (pnp_sched_sz == 0) return NULL_TASK; /*TODO: Error*/
    PNP_LOCK(back_lock);
    back = (pnp_sched_front + pnp_sched_sz - 1) % PNP_SCHED_TASK_MAX;
    ret = pnp_sched_task_deque[back];
    PNP_UNLOCK(back_lock);
    return ret;
}

void
pnp_push_back(pnp_sched_task_t task)
{
    uint32_t back;
    if (pnp_sched_sz == PNP_SCHED_TASK_MAX) return; /*TODO: Error*/
    PNP_LOCK(back_lock);
    back = (pnp_sched_front + pnp_sched_sz) % PNP_SCHED_TASK_MAX;
    pnp_sched_task_deque[back] = task;
    pnp_sched_sz++;
    PNP_UNLOCK(back_lock);
}

pnp_sched_task_t
pnp_pop_front(void)
{
    pnp_sched_task_t ret;
    if (pnp_sched_sz == 0) return NULL_TASK; /*TODO: Error*/
    PNP_LOCK(front_lock);
    ret = pnp_sched_task_deque[pnp_sched_front];
    pnp_sched_front = (pnp_sched_front + 1) % PNP_SCHED_TASK_MAX;
    pnp_sched_sz--;
    PNP_UNLOCK(front_lock);
    return ret;
}

pnp_sched_task_t
pnp_peek_front(void)
{
    if (pnp_sched_sz == 0) return NULL_TASK; /*TODO: Error*/
    return pnp_sched_task_deque[pnp_sched_front];
}

void
pnp_push_front(pnp_sched_task_t task)
{
    if (pnp_sched_sz == PNP_SCHED_TASK_MAX) return; /*TODO: Error*/
    PNP_LOCK(front_lock);
    pnp_sched_front = pnp_sched_front - 1 + PNP_SCHED_TASK_MAX;
    pnp_sched_front %= PNP_SCHED_TASK_MAX;
    pnp_sched_task_deque[pnp_sched_front] = task;
    pnp_sched_sz++;
    PNP_UNLOCK(front_lock);
}

