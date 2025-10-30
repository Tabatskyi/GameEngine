#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "GameObject.hpp"

class Scene
{
public:
	void Add(const GameObject& object);
	void Add(std::unique_ptr<GameObject> object);
	void Update(Uint32 deltaMs, const Uint8* keyboardState, int screenWidth, int screenHeight);
	void Render(SDL_Renderer* renderer) const;

private:
	std::vector<std::unique_ptr<GameObject>> objects;
};
