#include "Display.h"

bool is_running = false;
int previous_frame_time = 0;

void setup(void)
{
    render_method = RENDER_FILL_TRIANGLE;
    cull_method = CULL_BACKFACE;

    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * win_width * win_height);
    if (!color_buffer)
    {
        fprintf(stderr, "Error initializing color buffer.\n");
    }

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        win_width, win_height
    );
}

void process_input(void)
{
    SDL_Event input_event;
    SDL_PollEvent(&input_event);
    switch (input_event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;

    case SDL_KEYDOWN:
        switch (input_event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            is_running = false;
            break;

        case SDLK_1:
            render_method = RENDER_WIRE_VERTEX;
            break;

        case SDLK_2:
            render_method = RENDER_WIRE;
            break;

        case SDLK_3:
            render_method = RENDER_FILL_TRIANGLE;
            break;

        case SDLK_4:
            render_method = RENDER_FILL_TRIANGLE_WIRE;
            break;

        case SDLK_c:
            cull_method = CULL_BACKFACE;
            break;

        case SDLK_d:
            cull_method = CULL_NONE;
            break;
        }
        break;
    }
}

void update(void)
{
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();
}

void render(void)
{
    draw_grid(50, 0xFF333333);
    draw_solid_circle(200, 200, 40, 0xFFFFFF00);

    render_color_buffer();

    // clear_color_buffer(0x6495ED);
    clear_color_buffer(0x00000000);

    SDL_RenderPresent(renderer);
}

void free_resources(void)
{
    free(color_buffer);
}

int main(int argc, char* args[])
{
    is_running = initialize_window();
    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();
    return 0;
}
