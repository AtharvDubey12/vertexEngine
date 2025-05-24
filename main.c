// main.c
#include "include/tinysdl.h"
#include <Windows.h>
#include <time.h>

#define SPRITE_SCALE 2
#define TILE_SIZE 16
#define MAP_WIDTH 20
#define MAP_HEIGHT 15

float player_pos_x = 1 * TILE_SIZE;
float player_pos_y = 1 * TILE_SIZE;

int player_tile_x = 1;
int player_tile_y = 1;

int is_moving = 0;
float move_speed = 200.0f; // pixels per second
int move_dir_x = 0;
int move_dir_y = 0;

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
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
const uint32_t player_sprite_data[8 * 8] = {
    COLOR_RGB(255, 0, 0), COLOR_RGB(255, 0, 0), 0, 0, 0, 0, COLOR_RGB(255, 0, 0), COLOR_RGB(255, 0, 0),
    COLOR_RGB(255, 0, 0), COLOR_RGB(255, 255, 255), 0, 0, 0, 0, COLOR_RGB(255, 255, 255), COLOR_RGB(255, 0, 0),
    0, COLOR_RGB(255, 255, 255), COLOR_RGB(255, 255, 255), 0, 0, COLOR_RGB(255, 255, 255), COLOR_RGB(255, 255, 255), 0,
    0, 0, COLOR_RGB(0, 0, 0), COLOR_RGB(255, 255, 255), COLOR_RGB(255, 255, 255), COLOR_RGB(0, 0, 0), 0, 0,
    0, COLOR_RGB(0, 0, 0), 0, 0, 0, 0, COLOR_RGB(0, 0, 0), 0,
    0, COLOR_RGB(0, 0, 0), 0, 0, 0, 0, COLOR_RGB(0, 0, 0), 0,
    0, 0, COLOR_RGB(0, 0, 0), 0, 0, COLOR_RGB(0, 0, 0), 0, 0,
    0, 0, 0, COLOR_RGB(0, 0, 0), COLOR_RGB(0, 0, 0), 0, 0, 0
};

Sprite player_sprite = {
    .width = 8,
    .height = 8,
    .pixels = player_sprite_data
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
    clock_t last_time = clock();

    while (!tsdl_should_close()) {
        tsdl_poll_events();


        clock_t current_time = clock();
        float delta_time = (float)(current_time - last_time) / CLOCKS_PER_SEC;
        last_time = current_time;

        if (!is_moving) {
            if (GetAsyncKeyState(VK_UP) & 0x8000) {
                if (can_move_to(player_tile_x, player_tile_y - 1)) {
                    move_dir_x = 0;
                    move_dir_y = -1;
                    is_moving = 1;
                }
            }
            else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                if (can_move_to(player_tile_x, player_tile_y + 1)) {
                    move_dir_x = 0;
                    move_dir_y = 1;
                    is_moving = 1;
                }
            }
            else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                if (can_move_to(player_tile_x - 1, player_tile_y)) {
                    move_dir_x = -1;
                    move_dir_y = 0;
                    is_moving = 1;
                }
            }
            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                if (can_move_to(player_tile_x + 1, player_tile_y)) {
                    move_dir_x = 1;
                    move_dir_y = 0;
                    is_moving = 1;
                }
            }
        }

     
        if (is_moving) {
            player_pos_x += move_dir_x * move_speed * delta_time;
            player_pos_y += move_dir_y * move_speed * delta_time;

            float target_x = player_tile_x * TILE_SIZE + move_dir_x * TILE_SIZE;
            float target_y = player_tile_y * TILE_SIZE + move_dir_y * TILE_SIZE;

    
            if ((move_dir_x != 0 && ((move_dir_x > 0 && player_pos_x >= target_x) || (move_dir_x < 0 && player_pos_x <= target_x))) ||
                (move_dir_y != 0 && ((move_dir_y > 0 && player_pos_y >= target_y) || (move_dir_y < 0 && player_pos_y <= target_y)))) {
         
                player_pos_x = target_x;
                player_pos_y = target_y;

            
                player_tile_x += move_dir_x;
                player_tile_y += move_dir_y;

              
                is_moving = 0;
                move_dir_x = 0;
                move_dir_y = 0;
            }
        } else {

            player_pos_x = player_tile_x * TILE_SIZE;
            player_pos_y = player_tile_y * TILE_SIZE;
        }

        // Clear screen to black
        TSDL_Framebuffer* fb = tsdl_get_framebuffer();
        for (int i = 0; i < fb->width * fb->height; i++) {
            fb->pixels[i] = COLOR_RGB(0, 0, 0);
        }

        // Draw tilemap
        for (int row = 0; row < MAP_HEIGHT; row++) {
            for (int col = 0; col < MAP_WIDTH; col++) {
                int tile = tilemap[row][col];
                unsigned int color = get_tile_color(tile);
                tsdl_draw_rect(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
            }
        }

        tsdl_draw_sprite(&player_sprite, (int)player_pos_x, (int)player_pos_y, SPRITE_SCALE);


        tsdl_present();
        tsdl_sleep(16); //60fps lim
    }

    tsdl_shutdown();
    return 0;
}
