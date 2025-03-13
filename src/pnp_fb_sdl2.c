#include <SDL.h>
typedef struct sdl_fb_s sdl_fb_t;
struct sdl_fb_s
{
    void (*draw)(sdl_fb_t* self);
    void (*swap)(sdl_fb_t* self);
    int writable_pitch;
    void * pixel_data;
    SDL_Texture *fbs[2];
    SDL_Renderer *r;
    uint8_t fbi;
};

void
sdl_fb_draw(sdl_fb_t *ctx);
void
sdl_fb_swap(sdl_fb_t *ctx);
void *
sdl_fb_get(sdl_fb_t *ctx);

size_t
sdl_fb_init(sdl_fb_t *fb, SDL_Renderer *r,uint32_t pixel_fmt, int w, int h)
{
    int pitch;
    if (!fb) return sizeof(sdl_fb_t);
    fb->r = r;
    fb->fbs[0] = SDL_CreateTexture(
        r,
        pixel_fmt,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );

    fb->fbs[1] = SDL_CreateTexture(
        r,
        pixel_fmt,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );
    if (!fb->fbs[0] || !fb->fbs[1]) goto error;
    
    /* make current drawable fb writable */
    SDL_LockTexture(
        fb->fbs[fb->fbi],
        NULL,
        &fb->pixel_data,
        &pitch
    );
    fb->fbi = 0;
    fb->draw = sdl_fb_draw;
    fb->swap = sdl_fb_swap;

    return sizeof(sdl_fb_t);

error:
    SDL_DestroyTexture(fb->fbs[0]);
    SDL_DestroyTexture(fb->fbs[1]);
    return 0;
}

void
sdl_fb_draw(sdl_fb_t *ctx)
{
    SDL_RenderCopy(
        ctx->r,
        ctx->fbs[ctx->fbi],
        NULL, NULL
    );
    SDL_RenderPresent(ctx->r);
}

void
sdl_fb_swap(sdl_fb_t *ctx)
{
    int pitch;
    /* make current drawable fb writable */
    SDL_LockTexture(
        ctx->fbs[ctx->fbi],
        NULL,
        &ctx->pixel_data,
        &pitch
    );
    /* make current writable fb drawable */
    SDL_UnlockTexture(ctx->fbs[!ctx->fbi]);
    /* swap framebuffers*/
    ctx->fbi = !ctx->fbi;
    ctx->writable_pitch = pitch;
   
}

void *
sdl_fb_get(sdl_fb_t *ctx)
{
    return ctx->pixel_data;
}

