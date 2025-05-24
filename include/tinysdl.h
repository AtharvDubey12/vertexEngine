#ifndef TINYSDL_H
#define TINYSDL_H

#ifdef __cplusplus
extern "C" {
#endif

#define COLOR_RGBA(r, g, b, a) ((a << 24) | (r << 16) | (g << 8) | (b))
#define COLOR_RGB(r, g, b)     COLOR_RGBA(r, g, b, 255)


typedef struct {
    unsigned char keys[256];
    int mouse_x, mouse_y;
    int mouse_down;
} TSDL_Input;

typedef struct {
    int width;
    int height;
    unsigned int* pixels;  // RGBA pixels
} TSDL_Framebuffer;

int tsdl_init(const char* title, int width, int height);
void tsdl_shutdown();

void tsdl_poll_events();
int tsdl_should_close();

TSDL_Input* tsdl_get_input();
TSDL_Framebuffer* tsdl_get_framebuffer();
void tsdl_present();

double tsdl_get_time();
void tsdl_sleep(int ms);
void tsdl_draw_pixel(int x, int y, unsigned int color);
void tsdl_draw_rect(int x, int y, int w, int h, unsigned int color);
#include "sprite.h"
void tsdl_draw_sprite(const Sprite* sprite, int x, int y, int scale);


#ifdef __cplusplus
}
#endif

#endif // TINYSDL_H
