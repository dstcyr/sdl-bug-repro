#include "Display.h"
#include <string.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;
int win_width = 1920;
int win_height = 1080;
bool use_fullscreen_mode = false;
bool use_fixed_resolution = true;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    if (!use_fixed_resolution)
    {
        SDL_DisplayMode display_mode;
        SDL_GetCurrentDisplayMode(0, &display_mode);
        win_width = display_mode.w;
        win_height = display_mode.h;
    }

    window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        win_width, win_height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window)
    {
        fprintf(stderr, "Error initializing SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error initializing SDL renderer.\n");
        return false;
    }

    if (use_fullscreen_mode)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    return true;
}

void draw_grid(int cell_size, uint32_t color)
{
    for (int y = 0; y < win_height; y++)
    {
        for (int x = 0; x < win_width; x++)
        {
            if (x % cell_size == 0 || y % cell_size == 0)
            {
                draw_pixel(x, y, color);
            }
        }
    }
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x < win_width && y < win_height && x >= 0 && y >= 0)
    {
        color_buffer[(win_width * y) + x] = color;
    }
}

void draw_solid_rectangle(int x, int y, int w, int h, uint32_t color)
{
    for (int ry = y; ry < y + h; ry++)
    {
        for (int rx = x; rx < x + w; rx++)
        {
            draw_pixel(rx, ry, color);
        }
    }
}

void draw_rectangle(int x, int y, int w, int h, uint32_t color)
{
    int max_x = x + w;
    int max_y = y + h;

    for (int yy = y; yy < max_y; yy++)
    {
        for (int xx = x; xx < max_x; xx++)
        {
            if (xx == x || yy == y || xx == max_x - 1 || yy == max_y - 1)
            {
                draw_pixel(xx, yy, color);
            }
        }
    }
}

void draw_circle(int x, int y, int r, uint32_t color)
{
    int cx = x;
    int cy = y;
    double angle = 0.0;

    while (angle < 2 * M_PI)
    {
        int tx = (int)(cx + r * cos(angle));
        int ty = (int)(cy + r * sin(angle));

        if (tx >= 0 && tx < win_width && ty >= 0 && ty < win_height)
        {
            draw_pixel(tx, ty, color);
        }
        angle += 2 * M_PI / (r * 10);
    }
}

void draw_solid_circle(int x, int y, int r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++)
    {
        for (int dx = -r; dx <= r; dx++)
        {
            int distanceSquared = dx * dx + dy * dy;
            if (distanceSquared <= r * r)
            {
                int tx = x + dx;
                int ty = y + dy;

                if (tx >= 0 && tx < win_width && ty >= 0 && ty < win_height)
                {
                    draw_pixel(tx, ty, color);
                }
            }
        }
    }
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < win_height; y++)
    {
        for (int x = 0; x < win_width; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void render_color_buffer()
{
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(win_width * sizeof(uint32_t))
    );

    SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        NULL, NULL
    );
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);
    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; i++)
    {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void int_swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inv_slope_1 = (float)(x1 - x0) / (float)(y1 - y0);
    float inv_slope_2 = (float)(x2 - x0) / (float)(y2 - y0);

    float x_start = x0;
    float x_end = x0;

    for (int y = y0; y <= y2; y++)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inv_slope_1 = (float)(x2 - x0) / (float)(y2 - y0);
    float inv_slope_2 = (float)(x2 - x1) / (float)(y2 - y1);

    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y0; y--)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }

    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        int my = y1;
        int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
        fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
        fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
    }
}
