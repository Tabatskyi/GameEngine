#include <SDL2/SDL.h>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <algorithm>

constexpr const unsigned int SCREEN_WIDTH = 800;
constexpr const unsigned int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not be initialized!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            std::cout << "Renderer could not be created!" << std::endl
                << "SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_Rect squareRect;

            squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
            squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

            bool quit = false;

            while (!quit)
            {
                SDL_Event e;

                SDL_WaitEvent(&e);

                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

                SDL_RenderFillRect(renderer, &squareRect);

                SDL_RenderPresent(renderer);
            }

            SDL_DestroyRenderer(renderer);
        }

        SDL_DestroyWindow(window);
    }

    SDL_Quit();

    return 0;
}