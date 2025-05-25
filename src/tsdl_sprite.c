
#include "../include/tinysdl.h"
#include "../include/sprite.h"


void tsdl_draw_sprite(const Sprite* sprite, int x, int y, int scale) {
    TSDL_Framebuffer* fb = tsdl_get_framebuffer();

    for (int row = 0; row < sprite->height; row++) {
        for (int col = 0; col < sprite->width; col++) {
            uint32_t color = sprite->pixels[row * sprite->width + col];
            if ((color >> 24) == 0) continue;

            for (int dy = 0; dy < scale; dy++) {
                for (int dx = 0; dx < scale; dx++) {
                    int screen_x = x + col * scale + dx;
                    int screen_y = y + row * scale + dy;

                    if (screen_x >= 0 && screen_y >= 0 && screen_x < fb->width && screen_y < fb->height) {
                        fb->pixels[screen_y * fb->width + screen_x] = color;
                    }
                }
            }
        }
    }
}

