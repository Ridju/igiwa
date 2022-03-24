#include "display.h"
#include <stdio.h>
#include <SDL2/SDL.h>


Display init_display() {
    Display display;
    display.display = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        DISPLAY_WIDTH * PIXEL_SIZE, DISPLAY_HEIGHT * PIXEL_SIZE, SDL_WINDOW_OPENGL);
    if(display.display == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
    }
    display.renderer = SDL_CreateRenderer(display.display, -1, 0);
    SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, 255);
    SDL_RenderClear(display.renderer);
    SDL_RenderPresent(display.renderer);
    display.draw_flag = 1;
    for(int y = 0; y < DISPLAY_HEIGHT; y++){
        for(int x = 0; x < DISPLAY_WIDTH; x++){
            display.pixels[x][y] = 0;
        }
    }
    return display;
}

int draw_pixel(SDL_Renderer *renderer, int x, int y, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect rect;
    rect.x = x * PIXEL_SIZE;
    rect.y = y * PIXEL_SIZE;
    rect.w =  PIXEL_SIZE;
    rect.h = PIXEL_SIZE;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);
    return 1;
}

int add_sprite(Display *display, int x, int y,  uint8_t sprite){
    int flipped = 0;
    for(int i = 0; i <= PIXEL_WIDTH; i++){
        if((x + i) > DISPLAY_WIDTH) break;
        int old_value = display->pixels[(x+i)][y];
        display->pixels[x+i][y] = display->pixels[x+i][y] ^ ((sprite >> (PIXEL_WIDTH - i)) & 0x01);
        int new_value = display->pixels[(x+i)][y];
        for(int j = 0; j < PIXEL_WIDTH; j++) {
            if((old_value >> j) == 1 && (new_value >> j) == 0){
                flipped = 1;
            }
        }
    }
    return flipped;
}

int draw(Display *display){
    SDL_RenderClear(display->renderer);
    for(int y = 0; y < DISPLAY_HEIGHT; y++){
        for(int x = 0; x < DISPLAY_WIDTH; x++){
            if(display->pixels[x][y] == 1){
                draw_pixel(display->renderer, x, y, white);
            } else {
                draw_pixel(display->renderer, x, y, black);
            }
        }
    }
    SDL_RenderPresent(display->renderer);
}

int clear_display(Display *display){
    for(int y = 0; y < DISPLAY_HEIGHT; y++){
        for(int x = 0; x < DISPLAY_WIDTH; x++){
            display->pixels[x][y] = 0;
            draw_pixel(display->renderer, x, y, black);
        }
    }
    return 1;
}
