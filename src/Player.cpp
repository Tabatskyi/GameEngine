#include "Player.hpp"

void Player::Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboardState)
{
	double dirX = 0.0, dirY = 0.0;
	if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) dirX -= 1.0f;
	if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) dirX += 1.0f;
	if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) dirY -= 1.0f;
	if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) dirY += 1.0f;

	if (dirX != 0.0 || dirY != 0.0)
	{
		double length = std::sqrt(dirX * dirX + dirY * dirY);
		if (length > 0.0) 
		{ 
			dirX /= length; 
			dirY /= length; 
		}
	}

	posX += dirX * speed * static_cast<double>(deltaMs);
	posY += dirY * speed * static_cast<double>(deltaMs);

	posX = std::max(0.0, std::min(static_cast<double>(screenWidth - rect.w), posX));
	posY = std::max(0.0, std::min(static_cast<double>(screenHeight - rect.h), posY));
	rect.x = static_cast<int>(std::lround(posX));
	rect.y = static_cast<int>(std::lround(posY));
}

void Player::OnCollision(GameObject& other)
{
	Enemy* enemy = dynamic_cast<Enemy*>(&other);
	if (enemy && scene)
	{
		scene->TriggerGameOver(enemy);
	}
}