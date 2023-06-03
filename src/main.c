#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] =
    {
        SDLK_1, SDLK_2, SDLK_3, SDLK_4,
        SDLK_q, SDLK_w, SDLK_e, SDLK_r,
        SDLK_a, SDLK_s, SDLK_d, SDLK_f,
        SDLK_z, SDLK_x, SDLK_c, SDLK_v
    };

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("You need to have a file to load\n");
        return -1;
    }

    const char *filename = argv[1];
    printf("%s file is loaded\n", filename);

    FILE *f = fopen(filename, "rb");
    if(!f)
    {
        printf("Failed to open file\n");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f,0,SEEK_SET);

    char buf[size];
    int res = fread(buf, size, 1, f);
    if(res != 1)
    {
        printf("Failed to read from file\n");
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, buf, size);
    chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                goto out;
                break;

            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    chip8_keyboard_down(&chip8.keyboard, vkey);
                }
            }
            break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    chip8_keyboard_up(&chip8.keyboard, vkey);
                }
            }
            break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (chip8_screen_is_set(&chip8.screen, x, y))
                {
                    SDL_Rect rectangle;
                    rectangle.x = x * CHIP8_WINDOW_MULTIPLIER;
                    rectangle.y = y * CHIP8_WINDOW_MULTIPLIER;
                    rectangle.w = CHIP8_WINDOW_MULTIPLIER;
                    rectangle.h = CHIP8_WINDOW_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &rectangle);
                }
            }
        }
        SDL_RenderPresent(renderer);

        if(chip8.registers.delay_timer > 0)
        {
            Sleep(1);
            chip8.registers.delay_timer -= 1;
        }

        if(chip8.registers.sound_timer > 0)
        {
            Beep(15000, 10 * chip8.registers.sound_timer);
            chip8.registers.sound_timer = 0;
        }

        unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        chip8_exec(&chip8, opcode);
    }
out:
    SDL_DestroyWindow(window);
    return 0;
}