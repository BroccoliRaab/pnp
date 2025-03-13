#ifndef PNP_FB_H
#define PNP_FB_H

#include <stdint.h>

typedef struct fb_ctx_s pnp_fb_ctx_t;
struct pnp_fb_ctx_s
{
    void (*draw)(pnp_fb_ctx_t);
    void (*swap)(pnp_fb_ctx_t);
    void (*get)(pnp_fb_ctx_t);
    int writable_pitch;
    void * pixel_data;
};

#endif /* PNP_FB_H */
