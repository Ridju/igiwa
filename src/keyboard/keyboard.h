#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H
#include "SDL2/SDL.h"

struct Keyboard{
    int input_keys[16];
    size_t input_size;
    int running;
};

typedef struct Keyboard Keyboard;

enum {
    KEY_X = 0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_Z,
    KEY_C,
    KEY_4,
    KEY_R,
    KEY_F,
    KEY_V
};

Keyboard init_keyboard();
void get_keys(SDL_Event e, Keyboard *keyboard);
#endif //CHIP8_KEYBOARD_H
