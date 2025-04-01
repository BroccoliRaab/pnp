#include "pnp_platform.h"
#include "pnp_sched.h"
#include "pnp_renderer.h"
#include "pnp_renderer_sdl2.h"



void 
entry(void * arg);
void
exit_task(void * arg);
void
green_task(void * arg);



int main(void)
{
    pnp_renderer_sdl2_t r;
    if (sdl_renderer_init(&r, 320, 280)) return 1;
    pnp_sched_init(
        (pnp_sched_task_t){
            entry,
            &r
        }
    );
    pnp_sched_worker();
}

void 
entry(void * arg)
{
    pnp_renderer_t *r = (pnp_renderer_t *) arg;
    r->palette[0] = 0x00FF00FF;


    pnp_push_front(
        (pnp_sched_task_t){
            green_task,
            r
        }
    );
}

void
exit_task(void * arg)
{

}

void
green_task(void * arg)
{
    pnp_renderer_t *r = (pnp_renderer_t *) arg;
    uint16_t spani;
    for (spani = 0; spani < r->h; spani++)
        r->draw_span(r, 0, spani, 0, r->w);
    r->draw(r);
    pnp_push_front(
        (pnp_sched_task_t){
            green_task,
            r
        }
    );
}
