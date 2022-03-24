#include "cpu.h"
#include "../display/display.h"
#include "../keyboard/keyboard.h"

#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

CPU init_cpu(char *rom_path) {
    CPU cpu;
    init_memory(&cpu, rom_path);
    cpu.program_counter = PC_START;
    cpu.stack_pointer = 0;
    cpu.sound_timer = 0;
    cpu.delay_timer = 0;
    cpu.counter = 0;
    cpu.timer = 1.0 / FREQUENCY;

    for (int i = 0; i < 80; i++) {
        cpu.ram[i] = chip8_fontset[i];
    }

    return cpu;
}

int init_memory(struct CPU *cpu, const char *rom_path) {
    FILE *file = fopen(rom_path, "rb");

    if (!file) {
        perror("fopen");
        return 0;
    }
    fread(&cpu->ram[PC_START], sizeof(unsigned short), 4096 - 512, file);

    fclose(file);
}

uint16_t get_instruction(struct CPU *cpu) {
    unsigned char value = cpu->ram[cpu->program_counter];
    unsigned char value2 = cpu->ram[cpu->program_counter + 1];
    uint16_t instruction = value << 8 | value2;
    return instruction;
}

void zero_opcode(uint16_t instruction, Display *display, CPU *cpu) {
    switch (instruction & 0x0FFF) {
        case 0x00E0:
            clear_display(display);
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x00EE:
            cpu->program_counter = cpu->stack[cpu->stack_pointer];
            if(cpu->stack_pointer > 0) cpu->stack_pointer--;
            cpu->program_counter += 2;
            break;
        default:
            printf("unknown opcode - OPCODE 0\n");
            break;
    }
}

void one_opcode(uint16_t instruction, CPU *cpu) {
    cpu->program_counter = instruction & 0X0FFF;
}

void two_opcode(uint16_t instruction, CPU *cpu) {
    cpu->stack_pointer++;
    cpu->stack[cpu->stack_pointer] = cpu->program_counter;
    cpu->program_counter = instruction & 0x0FFF;
}

void three_opcode(uint16_t instruction, CPU *cpu) {
    int reg = instruction >> 8 & 0xF;
    int compare_value = instruction & 0x00FF;
    if (cpu->registers[reg] == compare_value) {
        cpu->program_counter = cpu->program_counter + 4;
    } else {
        cpu->program_counter = cpu->program_counter + 2;
    }
}

void four_opcode(uint16_t instruction, CPU *cpu) {
    int reg = instruction >> 8 & 0xF;
    int compare_value = instruction & 0xFF;
    if (cpu->registers[reg] != compare_value) {
        cpu->program_counter = cpu->program_counter + 4;
    } else {
        cpu->program_counter = cpu->program_counter + 2;
    }
}

void five_opcode(uint16_t instruction, CPU *cpu) {
    int reg_x = instruction >> 8 & 0xF;
    int reg_y = instruction >> 4 & 0xF;
    if (cpu->registers[reg_x] == cpu->registers[reg_y]) {
        cpu->program_counter = cpu->program_counter + 4;
    } else {
        cpu->program_counter = cpu->program_counter + 2;
    }
}

void six_opcode(uint16_t instruction, CPU *cpu) {
    int reg = instruction >> 8 & 0xF;
    int value = instruction & 0xFF;
    cpu->registers[reg] = value;
    cpu->program_counter = cpu->program_counter + 2;
}

void seven_opcode(uint16_t instruction, CPU *cpu) {
    int reg = instruction >> 8 & 0xF;
    int value = instruction & 0xFF;
    cpu->registers[reg] = cpu->registers[reg] + value;
    cpu->program_counter = cpu->program_counter + 2;
}

void eight_opcode(uint16_t instruction, CPU *cpu) {
    int second_code = instruction & 0xF;
    int x = instruction >> 8 & 0xF;
    int y = instruction >> 4 & 0xF;
    switch (second_code) {
        case 0x0:
            cpu->registers[x] = cpu->registers[y];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x1:
            cpu->registers[x] = cpu->registers[x] | cpu->registers[y];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x2:
            cpu->registers[x] = cpu->registers[x] & cpu->registers[y];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x3:
            cpu->registers[x] = cpu->registers[x] ^ cpu->registers[y];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x4: {
            int first_value = cpu->registers[x];
            int second_value = cpu->registers[y];
            int result = first_value + second_value;
            if (result > 255) {
                cpu->registers[VF] = 1;
            } else {
                cpu->registers[VF] = 0;
            }
            cpu->registers[x] = result & 0xFF;
            cpu->program_counter = cpu->program_counter + 2;
        }
            break;
        case 0x5: {
            unsigned char first_value = cpu->registers[x];
            unsigned char second_value = cpu->registers[y];
            if (first_value > second_value) {
                cpu->registers[VF] = 1;
            } else {
                cpu->registers[VF] = 0;
            }
            cpu->registers[x] = first_value - second_value;
            cpu->program_counter = cpu->program_counter + 2;
        }
            break;
        case 0x6: {
            if (cpu->registers[x] && 0x1 == 1) {
                cpu->registers[VF] = 1;
            } else {
                cpu->registers[VF] = 0;
            }
            cpu->registers[x] = cpu->registers[x] >> 1;
            cpu->program_counter = cpu->program_counter + 2;
        }
            break;
        case 0x7: {
            int first_value = cpu->registers[y];
            int second_value = cpu->registers[x];
            if (first_value > second_value) {
                cpu->registers[VF] = 1;
            } else {
                cpu->registers[VF] = 0;
            }
            cpu->registers[x] = cpu->registers[y] - cpu->registers[x];
            cpu->program_counter = cpu->program_counter + 2;
        }
            break;
        case 0xE: {
            if (cpu->registers[x] && 0x1 == 1) {
                cpu->registers[VF] = 1;
            } else {
                cpu->registers[VF] = 0;
            }
            cpu->registers[x] = cpu->registers[x] << 1;
            cpu->program_counter = cpu->program_counter + 2;
        }
            break;
        default:
            printf("unknown opcode - OPCODE 8");
            break;
    }
}

void nine_opcode(uint16_t instruction, CPU *cpu) {
    int x = instruction >> 8 & 0xF;
    int y = instruction >> 4 & 0xF;

    if (cpu->registers[x] != cpu->registers[y]) {
        cpu->program_counter = cpu->program_counter + 4;
    } else {
        cpu->program_counter = cpu->program_counter + 2;
    }
}

void a_opcode(uint16_t instruction, CPU *cpu) {
    cpu->VI = instruction & 0x0FFF;
    cpu->program_counter = cpu->program_counter + 2;
}

void b_opcode(uint16_t instruction, CPU *cpu) {
    int value = instruction & 0x0FFF;
    cpu->program_counter = value + cpu->registers[V0];
}

void c_opcode(uint16_t instruction, CPU *cpu) {
    int x = instruction >> 8 & 0xF;
    srand(time(NULL));
    int value = instruction & 0x00FF;
    int random_val = rand() % 255;
    cpu->registers[x] = value & random_val;
    cpu->program_counter = cpu->program_counter + 2;
}

void d_opcode(uint16_t instruction, CPU *cpu, Display *display) {
    int x = instruction >> 8 & 0xF;
    int y = instruction >> 4 & 0xF;
    int n = instruction & 0xF;
    int flipped = 0;
    cpu->registers[VF] = 0;
    for (int i = 0; i < n; i++) {
        if((cpu->registers[y] + i) > DISPLAY_HEIGHT) break;
        flipped = add_sprite(display, (cpu->registers[x]) % DISPLAY_WIDTH, (cpu->registers[y] + i) % DISPLAY_HEIGHT, cpu->ram[cpu->VI + i]);
        if (flipped) {
            cpu->registers[VF] = flipped;
        }
    }
    display->draw_flag = 1;
    cpu->program_counter = cpu->program_counter + 2;
}

void e_opcode(uint16_t instruction, CPU *cpu, Keyboard *keyboard) {
    int x = instruction >> 8 & 0xF;
    int second_opcode = instruction & 0x00FF;
    int pressed_key;
    for(int i = 0; i < 16; i++) {
        if(keyboard->input_keys[i] == 1){
            pressed_key = i;
        }
    }
    switch (second_opcode) {
        case 0x9E:
            if (cpu->registers[x] == pressed_key) {
                cpu->program_counter = cpu->program_counter + 4;
            } else {
                cpu->program_counter = cpu->program_counter + 2;
            }
            break;
        case 0xA1:
            if (cpu->registers[x] != pressed_key) {
                cpu->program_counter = cpu->program_counter + 4;
            } else {
                cpu->program_counter = cpu->program_counter + 2;
            }
            break;
        default:
            printf("unknown opcode - OPCODE E");
            break;
    }
}

void f_opcode(uint16_t instruction, CPU *cpu, SDL_Event e, Keyboard *keyboard) {
    int second_opcode = instruction & 0xFF;
    int x = instruction >> 8 & 0xF;
    switch (second_opcode) {
        case 0x07:
            cpu->registers[x] = cpu->delay_timer;
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x0A: {
            int running = 1;
            while(running && keyboard->running) {
                if(SDL_PollEvent(&e) != 0) {
                    get_keys(e, keyboard);
                    for(int i = 0; i < keyboard->input_size; i++) {
                        if(keyboard->input_keys[i] == 1){
                            cpu->registers[x] = i;
                            keyboard->input_keys[i] = 0;
                            running = 0;
                            break;
                        }
                    }
                }
            }
            cpu->program_counter = cpu->program_counter + 2;
        }
            break;
        case 0x15:
            cpu->delay_timer = cpu->registers[x];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x18:
            cpu->sound_timer = cpu->registers[x];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x1E:
            cpu->VI = cpu->VI + cpu->registers[x];
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x29:
            cpu->VI = cpu->registers[x] * 5;
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x33:
            cpu->ram[cpu->VI] = (cpu->registers[x] / 100) % 10;
            cpu->ram[cpu->VI + 1] = (cpu->registers[x] / 10) % 10;
            cpu->ram[cpu->VI + 2] = cpu->registers[x] % 10;
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x55:
            for (int i = 0; i <= x; i++) {
                cpu->ram[cpu->VI + i] = cpu->registers[i];
            }
            cpu->program_counter = cpu->program_counter + 2;
            break;
        case 0x65:
            for (int i = 0; i <= x; i++) {
                cpu->registers[i] = cpu->ram[cpu->VI + i];
            }
            cpu->program_counter = cpu->program_counter + 2;
            break;
        default:
            printf("unknown opcode - OPCODE F");
            break;
    }
}