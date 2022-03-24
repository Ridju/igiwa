#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "../display/display.h"
#include "../keyboard/keyboard.h"

static uint16_t PC_START = 0x200;
static const FREQUENCY = 700;
enum {
    V0 = 0,
    V1,
    V2,
    V3,
    V4,
    V5,
    V6,
    V7,
    V8,
    V9,
    VA,
    VB,
    VC,
    VD,
    VE,
    VF,
};

static const unsigned char chip8_fontset[80] =
{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct CPU {
    unsigned char ram[4096];
    unsigned char registers[16];
    unsigned char delay_timer;
    unsigned char sound_timer;
    // address register
    uint16_t VI;
    uint16_t program_counter;
    unsigned short stack[16];
    unsigned short stack_pointer;
    double timer;
    int counter;
};

typedef struct CPU CPU;

CPU init_cpu(char *rom_path) ;
int init_memory(struct CPU *cpu, const char *rom_path);
uint16_t get_instruction(struct CPU *cpu);

//OPCODES
void zero_opcode(uint16_t instruction, Display *display, CPU *cpu);
void one_opcode(uint16_t instruction, CPU *cpu);
void two_opcode(uint16_t instruction, CPU *cpu);
void three_opcode(uint16_t instruction, CPU *cpu);
void four_opcode(uint16_t instruction, CPU *cpu);
void five_opcode(uint16_t instruction, CPU *cpu);
void six_opcode(uint16_t instruction, CPU *cpu);
void seven_opcode(uint16_t instruction, CPU *cpu);
void eight_opcode(uint16_t instruction, CPU *cpu);
void nine_opcode(uint16_t instruction, CPU *cpu);
void a_opcode(uint16_t instruction, CPU *cpu);
void b_opcode(uint16_t instruction, CPU *cpu);
void c_opcode(uint16_t instruction, CPU *cpu);
void d_opcode(uint16_t instruction, CPU *cpu, Display *display);
void e_opcode(uint16_t instruction, CPU *cpu, Keyboard *keyboard);
void f_opcode(uint16_t instruction, CPU *cpu, SDL_Event e, Keyboard *keyboard);

#endif //CHIP8_CPU_H
