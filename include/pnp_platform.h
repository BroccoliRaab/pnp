#ifndef PNP_PLATFORM_H
#define PNP_PLATFORM_H


#if PNP_TARGET_SDL3
/* TODO: version bump */
#include <SDL2/SDL.h>

#define PNP_INLINE SDL_INLINE
#define PNP_INVALID_LOCK (NULL)
#define PNP_GET_LOCK() ( SDL_CreateMutex() )
#define PNP_LOCK(lock) ( SDL_LockMutex(lock) )
#define PNP_UNLOCK(lock) ( SDL_UnlockMutex(lock) )
#define PNP_TRY_LOCK(lock) ( (uin8_t) SDL_UnlockMutex(lock) )
#define PNP_PIXEL_TYPE uint32_t
#define PNP_RGBA_PIXEL(r,g,b,a)       \
    ((uint32_t)((r) * 255.0)) << 24 ||\
    ((uint32_t)((g) * 255.0)) << 16 ||\
    ((uint32_t)((b) * 255.0)) << 8 

#define PNP_MAX(a,b) (a<b?b:a)
#define PNP_MIN(a,b) (b<a?b:a)
   
typedef SDL_mutex * pnp_lock_t;
typedef struct pnp_platform_ctx_s pnp_platform_ctx_t;

#elif PNP_TARGET_RP2350
#include <stdint.h>

#define PNP_INLINE __force_inline
#define PNP_INVALID_LOCK (0xFF)

typedef uint8_t pnp_lock_t;


#endif


#endif /* PNP_PLATFORM_H */
