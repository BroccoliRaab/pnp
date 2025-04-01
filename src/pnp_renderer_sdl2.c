#include "pnp_renderer_sdl2.h"

int 
sdl_renderer_init(pnp_renderer_sdl2_t *r, uint32_t w, uint32_t h)
{
    r->r.w = w;
    r->r.h = h;
    r->r.palette = r->palette;
    r->r.palette_sz = 64;
    r->r.draw = sdl_draw;
    r->r.draw_span = sdl_draw_span;
    r->r.deinit = sdl_renderer_deinit;

    r->sdl_win = SDL_CreateWindow(
        "pnp",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        r->r.w, r->r.h,
        0
    );
    if (!r->sdl_win) return 1;

    r->sdl_r = SDL_CreateRenderer(
        r->sdl_win, 
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (!r->sdl_r) return 1;

    r->sdl_tex = SDL_CreateTexture(
        r->sdl_r,
        SDL_PIXELFORMAT_RGBX8888,
        SDL_TEXTUREACCESS_STREAMING,
        r->r.w, r->r.h
    );
    if (!r->sdl_tex) return 1;
    SDL_LockTexture(
        r->sdl_tex,
        NULL,
        &r->tex_access,
        &r->tex_pitch
    );
    return 0;
}


void 
sdl_renderer_deinit(pnp_renderer_t *pnp_r)
{
    pnp_renderer_sdl2_t *r = (pnp_renderer_sdl2_t*) pnp_r;
    SDL_DestroyTexture(r->sdl_tex);
    SDL_DestroyRenderer(r->sdl_r);
    SDL_DestroyWindow(r->sdl_win);
    SDL_Quit();
}

void 
sdl_draw(
    pnp_renderer_t *pnp_r
)
{
    pnp_renderer_sdl2_t *r = (pnp_renderer_sdl2_t*) pnp_r;
    SDL_UnlockTexture(r->sdl_tex);
    SDL_RenderCopy(r->sdl_r, r->sdl_tex, NULL, NULL);
    SDL_RenderPresent(r->sdl_r);
    SDL_LockTexture(
        r->sdl_tex,
        NULL,
        &r->tex_access,
        &r->tex_pitch
    );
}

void 
sdl_draw_span(
    pnp_renderer_t *pnp_r, 
    uint16_t x, uint16_t y,
    uint8_t color_id, 
    uint16_t len
)
{
    pnp_renderer_sdl2_t *r = (pnp_renderer_sdl2_t*) pnp_r;
    uint8_t *start_8;
    uint32_t *start;
    uint16_t i;
    len = PNP_MIN(pnp_r->w-x, len);
    start_8 = r->tex_access;
    start_8 += y * r->tex_pitch +(x * sizeof(uint32_t));
    start = (uint32_t *) start_8;

    for (i = 0; i < len; i++)
        start[i] = r->palette[color_id];
}
