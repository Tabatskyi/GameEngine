#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Scene.hpp"
#include "GameObject.hpp"

class Engine
{
public:
	Engine(unsigned int width, unsigned int height);
	~Engine();

	bool Init();
	void Shutdown();
	void Run(Scene& scene);

	SDL_Renderer* GetRenderer() const { return renderer; }
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }

private:
	unsigned int width = 800;
	unsigned int height = 600;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool quit = false;
	bool autoPaused = false;
	bool manualPaused = false;
	Uint32 lastWindowEventTime = 0;
};