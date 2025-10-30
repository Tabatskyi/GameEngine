#include <glm/trigonometric.hpp>
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>

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
            std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_Rect squareRect{};

            squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 10;
            squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 10;

            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

            float posX = static_cast<float>(squareRect.x);
            float posY = static_cast<float>(squareRect.y);

            bool quit = false;
            bool autoPaused = false;
            bool manualPaused = false;
            Uint32 lastWindowEventTime;

            const float speed = 1.0f; 

            Uint32 lastTime = SDL_GetTicks();
            Uint32 currentTime;

            while (!quit)
            {
                SDL_Event e;

                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        if (e.key.keysym.sym == SDLK_ESCAPE)
                        {
                            quit = true;
                        }
                        else if (e.key.keysym.sym == SDLK_p && e.key.repeat == 0)
                        {
                            manualPaused = !manualPaused;
                        }
                    }
                    else if (e.type == SDL_WINDOWEVENT)
                    {
                        switch (e.window.event)
                        {
                            case SDL_WINDOWEVENT_FOCUS_LOST:
                            case SDL_WINDOWEVENT_MINIMIZED:
                                autoPaused = true;
                                SDL_ResetKeyboard();
                                break;
                            case SDL_WINDOWEVENT_FOCUS_GAINED:
                            case SDL_WINDOWEVENT_RESTORED:
                                autoPaused = false;
                                SDL_ResetKeyboard();
                                break;
                            case SDL_WINDOWEVENT_MOVED:
                            case SDL_WINDOWEVENT_RESIZED:
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                lastWindowEventTime = SDL_GetTicks();
                                SDL_ResetKeyboard();
                                break;
                            default:
                                break;
                        }
                    }
                }

                currentTime = SDL_GetTicks();
                Uint32 delta = currentTime - lastTime;

                Uint32 flags = SDL_GetWindowFlags(window);
                bool windowActive = (flags & SDL_WINDOW_MINIMIZED) == 0 && (flags & SDL_WINDOW_INPUT_FOCUS) != 0;

                bool draggingOrResizing = (lastWindowEventTime != 0) && (currentTime - lastWindowEventTime < 100);

                bool simPaused = manualPaused || autoPaused || !windowActive || draggingOrResizing;

                if (!simPaused)
                {
                    const Uint8* state = SDL_GetKeyboardState(nullptr);
                    float dirX = 0.0f;
                    float dirY = 0.0f;

                    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
                        dirX -= 1.0f;
                    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
                        dirX += 1.0f;
                    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
                        dirY -= 1.0f;
                    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
                        dirY += 1.0f;

                    if (dirX != 0.0f || dirY != 0.0f)
                    {
                        float len = std::sqrt(dirX * dirX + dirY * dirY);
                        if (len > 0.0f)
                        {
                            dirX /= len;
                            dirY /= len;
                        }
                    }

                    posX += dirX * speed * static_cast<float>(delta);
                    posY += dirY * speed * static_cast<float>(delta);

                    posX = std::max(0.0f, std::min(static_cast<float>(SCREEN_WIDTH - squareRect.w), posX));
                    posY = std::max(0.0f, std::min(static_cast<float>(SCREEN_HEIGHT - squareRect.h), posY));

                    squareRect.x = static_cast<int>(posX);
                    squareRect.y = static_cast<int>(posY);
                }

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(renderer, &squareRect);

                SDL_RenderPresent(renderer);

                SDL_Delay(1);

                lastTime = currentTime;
            }

            SDL_DestroyRenderer(renderer);
        }

        SDL_DestroyWindow(window);
    }

    SDL_Quit();

    return 0;
}