//
// Created by julian on 3/23/22.
//
#include "utils.h"

void calculate_timers(CPU *cpu) {
    if(cpu->counter % DELAY_TIMER_MOD == 0) {
        if (cpu->delay_timer > 0)
            --cpu->delay_timer;
        if (cpu->sound_timer > 0)
            --cpu->sound_timer;
        cpu->counter =0;
    }
    cpu->counter++;
}

void cpu_cycle(CPU *cpu, Display *display, SDL_Event event, Keyboard *keyboard){
    uint16_t instruction = get_instruction(cpu);
    int opcode = instruction & 0xF000;
    switch (opcode) {
        case 0x0000:
            zero_opcode(instruction, display, cpu);
            break;
        case 0x1000:
            one_opcode(instruction, cpu);
            break;
        case 0x2000:
            two_opcode(instruction, cpu);
            break;
        case 0x3000:
            three_opcode(instruction, cpu);
            break;
        case 0x4000:
            four_opcode(instruction, cpu);
            break;
        case 0x5000:
            five_opcode(instruction, cpu);
            break;
        case 0x6000:
            six_opcode(instruction, cpu);
            break;
        case 0x7000:
            seven_opcode(instruction, cpu);
            break;
        case 0x8000:
            eight_opcode(instruction, cpu);
            break;
        case 0x9000:
            nine_opcode(instruction, cpu);
            break;
        case 0xA000:
            a_opcode(instruction, cpu);
            break;
        case 0xB000:
            b_opcode(instruction, cpu);
            break;
        case 0xC000:
            c_opcode(instruction, cpu);
            break;
        case 0xD000:
            d_opcode(instruction, cpu, display);
            break;
        case 0xE000:
            e_opcode(instruction, cpu, keyboard);
            break;
        case 0xF000:
            f_opcode(instruction, cpu, event, keyboard);
            break;
        default:
            printf("unknown opcode - MAIN");
            break;
    }
}


void clear_all(Display *display) {
    SDL_DestroyWindow(display->display);
    SDL_DestroyRenderer(display->renderer);
}