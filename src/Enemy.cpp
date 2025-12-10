#include "Enemy.hpp"

Enemy::Enemy(int x, int y, int w, int h, Color color, double speed) : GameObject(x, y, w, h, color, speed) {}

void Enemy::Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard)
{
	if (target)
	{
		const SDL_Rect& targetRect = target->GetRect();
		
		double enemyCenterX = posX + rect.w / 2.0;
		double enemyCenterY = posY + rect.h / 2.0;
		double targetCenterX = targetRect.x + targetRect.w / 2.0;
		double targetCenterY = targetRect.y + targetRect.h / 2.0;

		double dirX = targetCenterX - enemyCenterX;
		double dirY = targetCenterY - enemyCenterY;

		double length = std::sqrt(dirX * dirX + dirY * dirY);
		if (length > 0.0)
		{
			dirX /= length;
			dirY /= length;
		}

		posX += dirX * speed * static_cast<double>(deltaMs);
		posY += dirY * speed * static_cast<double>(deltaMs);
	}

	posX = std::max(0.0, std::min(static_cast<double>(screenWidth - rect.w), posX));
	posY = std::max(0.0, std::min(static_cast<double>(screenHeight - rect.h), posY));
	rect.x = static_cast<int>(std::lround(posX));
	rect.y = static_cast<int>(std::lround(posY));
}

void Enemy::SetTarget(GameObject* newTarget)
{
	target = newTarget;
}