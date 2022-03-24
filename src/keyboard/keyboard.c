//
// Created by julian on 3/23/22.
//

#include "keyboard.h"
#include "SDL2/SDL.h"

Keyboard init_keyboard() {
    Keyboard keyboard;
    keyboard.running = 1;
    keyboard.input_size = 16;
    return keyboard;
}


void get_keys(SDL_Event e, Keyboard *keyboard) {
    if (e.type == SDL_QUIT) {
        keyboard->running =  0;
    }
    if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_x:
                keyboard->input_keys[KEY_X] = 1;
                break;
            case SDLK_1:
                keyboard->input_keys[KEY_1] = 1;
                break;
            case SDLK_2:
                keyboard->input_keys[KEY_2] = 1;
                break;
            case SDLK_3:
                keyboard->input_keys[KEY_3] = 1;
                break;
            case SDLK_q:
                keyboard->input_keys[KEY_Q] = 1;
                break;
            case SDLK_w:
                keyboard->input_keys[KEY_W] = 1;
                break;
            case SDLK_e:
                keyboard->input_keys[KEY_E] = 1;
                break;
            case SDLK_a:
                keyboard->input_keys[KEY_A] = 1;
                break;
            case SDLK_s:
                keyboard->input_keys[KEY_S] = 1;
                break;
            case SDLK_d:
                keyboard->input_keys[KEY_D] = 1;
                break;
            case SDLK_z:
                keyboard->input_keys[KEY_Z] = 1;
                break;
            case SDLK_c:
                keyboard->input_keys[KEY_C] = 1;
                break;
            case SDLK_4:
                keyboard->input_keys[KEY_4] = 1;
                break;
            case SDLK_r:
                keyboard->input_keys[KEY_R] = 1;
                break;
            case SDLK_f:
                keyboard->input_keys[KEY_F] = 1;
                break;
            case SDLK_v:
                keyboard->input_keys[KEY_V] = 1;
                break;
            default:
                break;
        }
    }
    if(e.type == SDL_KEYUP) {
        switch(e.key.keysym.sym) {
            case SDLK_x:
                keyboard->input_keys[KEY_X] = 0;
                break;
            case SDLK_1:
                keyboard->input_keys[KEY_1] = 0;
                break;
            case SDLK_2:
                keyboard->input_keys[KEY_2] = 0;
                break;
            case SDLK_3:
                keyboard->input_keys[KEY_3] = 0;
                break;
            case SDLK_q:
                keyboard->input_keys[KEY_Q] = 0;
                break;
            case SDLK_w:
                keyboard->input_keys[KEY_W] = 0;
                break;
            case SDLK_e:
                keyboard->input_keys[KEY_E] = 0;
                break;
            case SDLK_a:
                keyboard->input_keys[KEY_A] = 0;
                break;
            case SDLK_s:
                keyboard->input_keys[KEY_S] = 0;
                break;
            case SDLK_d:
                keyboard->input_keys[KEY_D] = 0;
                break;
            case SDLK_z:
                keyboard->input_keys[KEY_Z] = 0;
                break;
            case SDLK_c:
                keyboard->input_keys[KEY_C] = 0;
                break;
            case SDLK_4:
                keyboard->input_keys[KEY_4] = 0;
                break;
            case SDLK_r:
                keyboard->input_keys[KEY_R] = 0;
                break;
            case SDLK_f:
                keyboard->input_keys[KEY_F] = 0;
                break;
            case SDLK_v:
                keyboard->input_keys[KEY_V] = 0;
                break;
            default:
                break;
        }
    }
}