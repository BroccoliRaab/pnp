#ifndef PNP_RENDERER_SDL2_H
#define PNP_RENDERER_SDL2_H

#include "SDL.h"
#include "pnp_renderer.h"
#include "pnp_platform.h"



typedef struct pnp_renderer_sdl2_s pnp_renderer_sdl2_t;
struct pnp_renderer_sdl2_s
{
    pnp_renderer_t r;
    SDL_Renderer *sdl_r;
    SDL_Window *sdl_win;
    SDL_Texture *sdl_tex;
    uint32_t palette[64];
    void *tex_access;
    int tex_pitch;
};

int 
sdl_renderer_init(pnp_renderer_sdl2_t *r, uint32_t w, uint32_t h);

void 
sdl_draw(pnp_renderer_t *r);

void 
sdl_renderer_deinit(pnp_renderer_t *r);

void 
sdl_draw_span(
    pnp_renderer_t *pnp_r, 
    uint16_t x, uint16_t y,
    uint8_t color_id, 
    uint16_t len
);


#endif /* PNP_RENDERER_SDL2_H */
