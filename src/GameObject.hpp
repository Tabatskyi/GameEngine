#pragma once

#include <SDL2/SDL.h>
#include <cmath>

struct Color
{
	Uint8 r{255}, g{255}, b{255}, a{255};
	Color() = default;
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a =255) : r(r), g(g), b(b), a(a) {}
};

class GameObject
{
public:
	GameObject(int x, int y, int w, int h, Color color, float speed = 1.0f);
	virtual ~GameObject() = default;

	void SetVelocity(float vx, float vy);

	virtual void Update(Uint32 deltaMs, const Uint8* keyboard, int screenWidth, int screenHeight);
	virtual void Render(SDL_Renderer* renderer) const;

protected:
	SDL_Rect rect {};
	Color color {};
	float posX = 0.0f;
	float posY = 0.0f;
	float speed = 1.0f;
	float velX = 0.0f;
	float velY = 0.0f;
};