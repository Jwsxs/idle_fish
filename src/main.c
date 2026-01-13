#include <stdio.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define SDL_MAIN_HANDLED

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define FPS 60

void createWindow(SDL_Window **window);
void createRenderer(SDL_Renderer **renderer, SDL_Window **window);

void initializeSDL(SDL_Window **window, SDL_Renderer **renderer);
void initializeTexture(SDL_Renderer *renderer, SDL_Surface **surface, char *src_to_img, SDL_Texture **texture);

void fish_give_money(double mult);

double money = 0;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    initializeSDL(&window, &renderer);
    initializeTexture(renderer, &surface, "src/imgs/fish_default.png", &texture);

    SDL_Rect fish;
    SDL_QueryTexture(texture, NULL, NULL, &fish.w, &fish.h);
    fish.w /= 8;
    fish.h /= 8;

    float fish_x_pos = (WINDOW_WIDTH - fish.w) / 2;
    float fish_y_pos = (WINDOW_HEIGHT - fish.h) / 2;

    int fish_x_vel = 5;
    int fish_y_vel = 2;

    int quit_called = 0;

    double time = 60;

    while (!quit_called) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    printf("Closing. . .\n");
                    quit_called = 1;
                    break;
            }
        }

        if (time >= 0) {
            time--;
        } else {
            fish_give_money(((double)rand() / (double)RAND_MAX) + 1);
            time = 60 + (((double)rand() / (double)RAND_MAX) * 60);
            printf("time: %.1f\n", time);
        }

        if (fish.x >= (WINDOW_WIDTH - fish.w)) {
            fish_x_pos = (WINDOW_WIDTH - fish.w);
            fish_x_vel = -fish_x_vel;
        }
        if (fish.x <= 0) {
            fish_x_pos = 0;
            fish_x_vel = -fish_x_vel;
        }

        if (fish.y >= (WINDOW_HEIGHT - fish.h)) {
            fish_y_pos = (WINDOW_HEIGHT - fish.h);
            fish_y_vel = -fish_y_vel;
        }
        if (fish.y <= 0) {
            fish_y_pos = 0;
            fish_y_vel = -fish_y_vel;
        }

        fish_x_pos += fish_x_vel;
        fish_y_pos += fish_y_vel;

        fish.x = (int) fish_x_pos;
        fish.y = (int) fish_y_pos;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, texture, NULL, &fish, 0, NULL, fish_x_vel >= 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000/FPS);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void createWindow(SDL_Window **window) {
    *window = SDL_CreateWindow(
        __FILE__,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!*window) {
        fprintf(stderr, "error on window: %s\n", SDL_GetError());
    }
}

void createRenderer(SDL_Renderer **renderer, SDL_Window **window) {
    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!*renderer) {
        fprintf(stderr, "error on renderer: %s\n", SDL_GetError());
    }
}

void initializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "error on SDL: %s\n", SDL_GetError());
    }
    atexit(SDL_Quit);

    createWindow(window);
    createRenderer(renderer, window);
}

void initializeTexture(SDL_Renderer *renderer, SDL_Surface **surface, char *src_to_img, SDL_Texture **texture) {
    *surface = IMG_Load(src_to_img);
    if (!*surface) {
        fprintf(stderr, "error on surface: %s\n", SDL_GetError());
    }

    *texture = SDL_CreateTextureFromSurface(renderer, *surface);
    SDL_FreeSurface(*surface);
    if (!*texture) {
        fprintf(stderr, "error on texture: %s\n", SDL_GetError());
    }
}

int times_called = 0;
void fish_give_money(double mult) {
    times_called++;

    double range = (double)rand() / (double)RAND_MAX;
    // rand() returns a value between 0 and RAND_MAX
    // so dividing 32767 by 32767 (both RAND_MAX values) gives you 1
    // if it's less, 0.XX

    double add_amnt = range * mult;
    money += add_amnt;

    printf("\033[2J\033[H%i:\n  -> $%.2f * %.2f = +$%.2f\n  -> $%.2f\n\n", times_called, range, mult, add_amnt, money);
}
