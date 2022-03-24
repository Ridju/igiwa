#include "display/display.h"
#include "cpu/cpu.h"
#include "keyboard/keyboard.h"
#include "utils/utils.h"

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Event event;

    CPU cpu = init_cpu(argv[1]);
    Display display = init_display();
    Keyboard keyboard = init_keyboard();

    uint32_t end;
    double diff_time;

    while (keyboard.running) {
        uint32_t begin = SDL_GetPerformanceCounter();

        cpu_cycle(&cpu, &display, event, &keyboard);
        calculate_timers(&cpu);

        if(display.draw_flag) {
            draw(&display);
            display.draw_flag = 0;
        }

        end = SDL_GetPerformanceCounter();
        diff_time = (double)((end - begin) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
        while(diff_time < cpu.timer) {
            if(SDL_PollEvent(&event) != 0) {
                get_keys(event, &keyboard);
            }
            end = SDL_GetPerformanceCounter();
            diff_time = (double)((end - begin) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
        }

    }
    clear_all(&display);
    SDL_Quit();
    return 0;
}
