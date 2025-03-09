#ifndef PNP_PLATFORM_H
#define PNP_PLATFORM_H


#if PNP_TARGET_SDL3
#include <SDL2/SDL.h>

#define PNP_INLINE SDL_INLINE

#define PNP_INVALID_LOCK (NULL)
#define PNP_GET_LOCK() ( SDL_CreateMutex() )
#define PNP_LOCK(lock) ( SDL_LockMutex(lock) )
#define PNP_UNLOCK(lock) ( SDL_UnlockMutex(lock) )
#define PNP_TRY_LOCK(lock) ( (uin8_t) SDL_UnlockMutex(lock) )

typedef SDL_mutex * pnp_lock_t;


#elif PNP_TARGET_RP2350
#include <stdint.h>

#define PNP_INLINE __force_inline
#define PNP_INVALID_LOCK (0xFF)

typedef uint8_t pnp_lock_t;


#endif


#endif /* PNP_PLATFORM_H */
