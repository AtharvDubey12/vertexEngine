// main.c
#include "include/tinysdl.h"
#include<Windows.h>

#define TILE_SIZE 16
#define MAP_WIDTH 20
#define MAP_HEIGHT 15

int player_x = 1;
int player_y = 1;

int tilemap[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,0,1},
    {1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1},
    {1,0,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,0,1},
    {1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1},
    {1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1},
    {1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int can_move_to(int x, int y) {
    if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) return 0;
    return tilemap[y][x] == 0; // 0 = walkable
}


unsigned int get_tile_color(int tile_id) {
    switch (tile_id) {
        case 0: return COLOR_RGB(60, 180, 75);   // Grass
        case 1: return COLOR_RGB(120, 120, 120); // Wall
        default: return COLOR_RGB(255, 0, 0);    // Unknown
    }
}


int main() {
    tsdl_init("TinySDL Test", 320, 240);

while (!tsdl_should_close()) {
    tsdl_poll_events();

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
    if (can_move_to(player_x, player_y - 1)) player_y--;
}
if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
    if (can_move_to(player_x, player_y + 1)) player_y++;
}
if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
    if (can_move_to(player_x - 1, player_y)) player_x--;
}
if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
    if (can_move_to(player_x + 1, player_y)) player_x++;
}


    TSDL_Framebuffer* fb = tsdl_get_framebuffer();
    for (int i = 0; i < fb->width * fb->height; i++) {
        fb->pixels[i] = COLOR_RGB(0, 0, 0); // Clear screen
    }

    // tile map bana rha hai ye.
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            int tile = tilemap[row][col];
            unsigned int color = get_tile_color(tile);
            tsdl_draw_rect(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }
    tsdl_draw_rect(
    player_x * TILE_SIZE,
    player_y * TILE_SIZE,
    TILE_SIZE,
    TILE_SIZE,
    COLOR_RGB(255, 0, 0));


    tsdl_present();
    tsdl_sleep(16);
}


    tsdl_shutdown();
    return 0;
}
