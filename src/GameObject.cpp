#include "GameObject.hpp"
#include <algorithm>

GameObject::GameObject(int x, int y, int w, int h, Color color, float speed)
						: rect{ x, y, w, h }, color(color), posX(static_cast<float>(x)), posY(static_cast<float>(y)), speed(speed) {}

void GameObject::SetVelocity(float vx, float vy)
{
	velX = vx; velY = vy;
}

void GameObject::Update(Uint32 deltaMs, const Uint8*, int screenW, int screenH)
{
	posX += velX * speed * static_cast<float>(deltaMs);
	posY += velY * speed * static_cast<float>(deltaMs);
	if (posX <0.0f) 
	{ 
		posX =0.0f; 
		velX = std::fabs(velX);
	} 
	else if (posX > (screenW - rect.w)) 
	{ 
		posX = static_cast<float>(screenW - rect.w); 
		velX = -std::fabs(velX);
	} 
	if (posY < 0.0f) 
	{ 
		posY = 0.0f; 
		velY = std::fabs(velY);
	} 
	else if (posY > (screenH - rect.h)) 
	{ 
		posY = static_cast<float>(screenH - rect.h); 
		velY = -std::fabs(velY);
	} 

	posX = std::max(0.0f, std::min(static_cast<float>(screenW - rect.w), posX));
	posY = std::max(0.0f, std::min(static_cast<float>(screenH - rect.h), posY));
	rect.x = static_cast<int>(posX);
	rect.y = static_cast<int>(posY);
}

void GameObject::Render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}