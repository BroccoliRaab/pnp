#ifndef PNP_RENDERER_H
#define PNP_RENDERER_H

#include <stdint.h>

typedef struct pnp_renderer_s pnp_renderer_t;

typedef void(*draw_span_f)(pnp_renderer_t * r,uint16_t x, uint16_t y, uint8_t color_id, uint16_t len);  
typedef void(*renderer_deinit_f)(pnp_renderer_t * r);
typedef void(*draw_f)(pnp_renderer_t * r);

struct pnp_renderer_s
{
    draw_span_f draw_span;
    draw_f draw;
    renderer_deinit_f deinit;
    uint32_t w, h;
    uint32_t *palette;
    uint8_t palette_sz;
};

#endif /* PNP_RENDERER_H */
