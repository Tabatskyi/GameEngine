#include "Engine.hpp"

Engine::Engine(unsigned int width, unsigned int height) : width(width), height(height) {}

Engine::~Engine()
{
	Shutdown();
}

bool Engine::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not be initialized!\nSDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, static_cast<int>(width), static_cast<int>(height), SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void Engine::Shutdown()
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	SDL_Quit();
}

void Engine::Run(Scene& scene)
{
	if (!window || !renderer)
	{
		if (!Init()) return;
	}
	quit = false;
	autoPaused = false;
	manualPaused = false;
	lastWindowEventTime = 0;
	Uint32 lastTime = SDL_GetTicks();
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else if (event.key.keysym.sym == SDLK_p && event.key.repeat == 0)
				{
					manualPaused = !manualPaused;
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				switch (event.window.event)
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

		Uint32 currentTime = SDL_GetTicks();
		Uint32 delta = currentTime - lastTime;
		Uint32 flags = SDL_GetWindowFlags(window);

		bool windowActive = (flags & SDL_WINDOW_MINIMIZED) == 0 && (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
		bool draggingOrResizing = (lastWindowEventTime != 0) && (currentTime - lastWindowEventTime < 100);
		bool simPaused = manualPaused || autoPaused || !windowActive || draggingOrResizing;

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		if (!simPaused)
		{
			const Uint8* state = SDL_GetKeyboardState(nullptr);
			scene.Update(delta, state, static_cast<int>(width), static_cast<int>(height));
		}
		scene.Render(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
		lastTime = currentTime;
	}
}
