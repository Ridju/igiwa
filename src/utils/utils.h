#ifndef CHIP8_UTILS_H
#define CHIP8_UTILS_H

#include "../display/display.h"
#include "../cpu/cpu.h"

static const int DELAY_TIMER_MOD = 8;

void calculate_timers(CPU *cpu);
void cpu_cycle(CPU *cpu, Display *display, SDL_Event event, Keyboard *keyboard);
void clear_all(Display *display);

#endif 
