#include "GameObject.hpp"
#include "TextureUtils.hpp"

GameObject::GameObject(int x, int y, int w, int h, Color color, double speed)
	: rect{ x, y, w, h }, color(color), posX(static_cast<double>(x)), posY(static_cast<double>(y)), speed(speed) {}

GameObject::~GameObject()
{
	ClearTexture();
}

bool GameObject::altRenderEnabled = false;

void GameObject::SetAltRenderEnabled(bool enabled)
{
	altRenderEnabled = enabled;
}

bool GameObject::IsAltRenderEnabled()
{
	return altRenderEnabled;
}

void GameObject::SetVelocity(double vx, double vy)
{
	velX = vx; velY = vy;
}

void GameObject::SetTexture(std::shared_ptr<SDL_Texture> tex)
{
	texture = std::move(tex);
}

bool GameObject::InitTexture(SDL_Renderer* renderer, const std::string& path, int width, int height, int cell, SDL_Color color1, SDL_Color color2)
{
	texture = LoadTexture(renderer, path, width, height, cell, color1, color2);
	return texture != nullptr;
}

void GameObject::ClearTexture()
{
	texture.reset();
}

void GameObject::OffsetPosition(double dx, double dy)
{
	posX += dx;
	posY += dy;
	rect.x = static_cast<int>(std::lround(posX));
	rect.y = static_cast<int>(std::lround(posY));
}

void GameObject::Render(SDL_Renderer* renderer) const
{
	if (texture)
	{
		SDL_RenderCopy(renderer, texture.get(), nullptr, &rect);
		if (!altRenderEnabled)
			return;
	}
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}