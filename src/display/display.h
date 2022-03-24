#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H
#include <SDL2/SDL.h>

static const int DISPLAY_WIDTH = 64;
static const int DISPLAY_HEIGHT = 32;
static const int PIXEL_WIDTH = 8;
static const int PIXEL_SIZE = 12;

struct Display {
    SDL_Window *display;
    SDL_Renderer *renderer;
    int draw_flag;
    int pixels[64][32];
};

static const SDL_Color white = {255, 255, 255};
static const SDL_Color black = {0,0,0};


typedef struct Display Display;

Display init_display();
int draw_pixel(SDL_Renderer *renderer, int x, int y, SDL_Color color);
int add_sprite(Display *display, int x, int y,  uint8_t sprite);
int draw(Display *display);
int clear_display(Display *display);
#endif //CHIP8_DISPLAY_H
