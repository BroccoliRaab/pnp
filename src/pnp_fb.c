#include <pnp_platform.h>
#include <pnp_fb.h>
#include <stdio.h>

PNP_PIXEL_TYPE fb0[FB_WIDTH * FB_HEIGHT];
PNP_PIXEL_TYPE fb1[FB_WIDTH * FB_HEIGHT];
static PNP_PIXEL_TYPE * framebuffers[2] = {fb0, fb1};
static int current_framebuffer = 0;

void
pnp_fb_draw(pnp_platform_ctx_t *ctx)
{
    SDL_UnlockTexture(ctx->render_target);
    SDL_RenderCopy(
        ctx->renderer,
        ctx->render_target,
        NULL, NULL
    );
    SDL_RenderPresent(ctx->renderer);
}

int
pnp_fb_swap(void)
{
    current_framebuffer = !current_framebuffer;
}

PNP_PIXEL_TYPE *
pnp_fb_get_writable(pnp_platform_ctx_t *ctx)
{
    int pitch;
    SDL_LockTexture(
        ctx->render_target,
        NULL,
        framebuffers + !current_framebuffer,
        &pitch
    );
    printf("%d", pitch);
    return framebuffers[!current_framebuffer];
}

