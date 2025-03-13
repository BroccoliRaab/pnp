#include <SDL.h>
#include "pnp_platform.h"
#include "pnp_sched.h"
#include "pnp_fb.h"


void 
entry(void * arg);
void
exit_task(void * arg);
void
green_task(void * arg);

int main(void)
{
    pnp_platform_ctx_t platform_ctx;
    pnp_sched_init(
        (pnp_sched_task_t){
            entry,
            &platform_ctx
        }
    );
    pnp_sched_worker();
}

/* TODO: make this cross platform */
void 
entry(void * arg)
{
    pnp_platform_ctx_t *ctx = arg;
    ctx->win = NULL;
    ctx->renderer = NULL;
    ctx->render_target = NULL;
    
    ctx->win = SDL_CreateWindow(
        "pnp game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        FB_WIDTH,
        FB_HEIGHT,
        0
    );
    if(!ctx->win) goto error;
    
    ctx->renderer = SDL_CreateRenderer(
        ctx->win,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (!ctx->renderer) goto error;

    ctx->render_target = SDL_CreateTexture(
        ctx->renderer,
        SDL_PIXELFORMAT_RGBX8888,
        SDL_TEXTUREACCESS_STREAMING,
        FB_WIDTH,
        FB_HEIGHT
    );
    if (!ctx->render_target) goto error;

    pnp_push_back(
        (pnp_sched_task_t){
            green_task,
            ctx
        }
    );

    return;

error:
    pnp_push_front(
        (pnp_sched_task_t){
            exit_task,
            ctx
        }
    );
}

void
exit_task(void * arg)
{
    pnp_platform_ctx_t *ctx = arg;
    SDL_DestroyTexture(ctx->render_target);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->win);
    SDL_Quit();
}

void
green_task(void * arg)
{
    PNP_PIXEL_TYPE *fb;
    pnp_platform_ctx_t *ctx = arg;
    fb = pnp_fb_get_writable(ctx);
    memset(fb, PNP_RGBA_PIXEL(0.0, 1.0, 0.0, 0.0), sizeof(PNP_PIXEL_TYPE));
    pnp_fb_swap();
    pnp_fb_draw(ctx);
}
