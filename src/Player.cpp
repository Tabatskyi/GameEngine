#include "Player.hpp"
#include <cmath>
#include <algorithm>

void Player::Update(Uint32 deltaMs, const Uint8* keyboardState, int screenWidth, int screenHeight)
{
	float dirX = 0.0f, dirY = 0.0f;
	if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) dirX -= 1.0f;
	if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) dirX += 1.0f;
	if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) dirY -= 1.0f;
	if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) dirY += 1.0f;

	if (dirX != 0.0f || dirY != 0.0f)
	{
		float length = std::sqrt(dirX * dirX + dirY * dirY);
		if (length > 0.0f) { dirX /= length; dirY /= length; }
	}

	posX += dirX * speed * static_cast<float>(deltaMs);
	posY += dirY * speed * static_cast<float>(deltaMs);

	posX = std::max(0.0f, std::min(static_cast<float>(screenWidth - rect.w), posX));
	posY = std::max(0.0f, std::min(static_cast<float>(screenHeight - rect.h), posY));
	rect.x = static_cast<int>(posX);
	rect.y = static_cast<int>(posY);
}
