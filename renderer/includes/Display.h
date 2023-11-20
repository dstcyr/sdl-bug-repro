#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

enum cull_method
{
    CULL_NONE,
    CULL_BACKFACE
} cull_method;

enum render_method
{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} render_method;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;
extern uint32_t* color_buffer;
extern int win_width;
extern int win_height;
extern bool use_fullscreen_mode;
extern bool use_fixed_resolution;

bool initialize_window(void);
void draw_grid(uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_solid_rectangle(int x, int y, int w, int h, uint32_t color);
void draw_rectangle(int x, int y, int w, int h, uint32_t color);
void draw_circle(int x, int y, int r, uint32_t color);
void draw_solid_circle(int x, int y, int r, uint32_t color);
void clear_color_buffer(uint32_t color);
void render_color_buffer();
void destroy_window(void);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
