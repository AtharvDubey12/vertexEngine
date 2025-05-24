// include/sprite.h
#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

typedef struct {
    int width;
    int height;
    const uint32_t* pixels; // Pointer to color data
} Sprite;

#endif
