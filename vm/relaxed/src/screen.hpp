#ifndef ALTAIR_SCREEN_INTERNAL_H_DEFINED
#define ALTAIR_SCREEN_INTERNAL_H_DEFINED

#include "vm_relaxed.hpp"

typedef struct ArScreen_T
{
    ArVirtualMachine parent;
    ArGraphicsProcessor graphicsProcessor;
    struct SDL_Window* win;
    struct SDL_Renderer* renderer;
    struct SDL_Texture* texture;
    struct SDL_Surface* surface;
} ArScreen_T;

#endif
