#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <memory>
#include <string>

struct Color
{
	Uint8 r{255}, g{255}, b{255}, a{255};
	Color() = default;
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) : r(r), g(g), b(b), a(a) {}
};

class GameObject
{
public:
	GameObject(int x, int y, int w, int h, Color color, double speed = 1.0);
	virtual ~GameObject();

	void SetVelocity(double vx, double vy);

	void SetTexture(std::shared_ptr<SDL_Texture> texture);
	std::shared_ptr<SDL_Texture> GetTexture() const { return texture; }

	bool InitTexture(SDL_Renderer* renderer, const std::string& path, int width, int height, int cell,
		SDL_Color color1 = SDL_Color{255,0,255,255}, SDL_Color color2 = SDL_Color{0,0,0,255});

	void ClearTexture();

	virtual void Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard = nullptr);
	virtual void Render(SDL_Renderer* renderer) const;

	static void SetAltRenderEnabled(bool enabled);
	static bool IsAltRenderEnabled();

protected:
	SDL_Rect rect {};
	Color color {};
	double posX = 0.0;
	double posY = 0.0;
	double speed = 1.0;
	double velX = 0.0;
	double velY = 0.0;

	std::shared_ptr<SDL_Texture> texture = nullptr;

	static bool altRenderEnabled;
};