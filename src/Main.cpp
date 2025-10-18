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

            squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 10;
            squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 10;

            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

            bool quit = false;
            const int speed = 5; 

            while (!quit)
            {
                SDL_Event e;

                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                const Uint8* state = SDL_GetKeyboardState(nullptr);
                int dx = 0;
                int dy = 0;

                if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
                    dx -= speed;
                if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
                    dx += speed;
                if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
                    dy -= speed;
                if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
                    dy += speed;

                squareRect.x += dx;
                squareRect.y += dy;

                squareRect.x = std::max(0, std::min(static_cast<int>(SCREEN_WIDTH) - static_cast<int>(squareRect.w), squareRect.x));
                squareRect.y = std::max(0, std::min(static_cast<int>(SCREEN_HEIGHT) - static_cast<int>(squareRect.h), squareRect.y));

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(renderer, &squareRect);

                SDL_RenderPresent(renderer);

                SDL_Delay(16);
            }

            SDL_DestroyRenderer(renderer);
        }

        SDL_DestroyWindow(window);
    }

    SDL_Quit();

    return 0;
}