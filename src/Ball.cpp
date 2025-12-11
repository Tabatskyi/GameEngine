#include "Ball.hpp"
#include <algorithm>
#include <cmath>

Ball::Ball(int x, int y, int diameter, Color color, double speed)
	: GameObject(x, y, diameter, diameter, color, speed), initialSpeed(speed)
{
	SetVelocity(0.0, 0.0);
}

void Ball::Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard)
{
	(void)keyboard;

	if (!isActive)
	{
		SetVelocity(0.0, 0.0);
		return;
	}

	posX += velX * static_cast<double>(deltaMs);
	posY += velY * static_cast<double>(deltaMs);

	if (posX <= 0.0)
	{
		posX = 0.0;
		velX = std::abs(velX) * bounceFactor;
		SetVelocity(velX, velY);
	}
	else if (posX + rect.w >= screenWidth)
	{
		posX = screenWidth - rect.w;
		velX = -std::abs(velX) * bounceFactor;
		SetVelocity(velX, velY);
	}

	if (posY <= 0.0)
	{
		posY = 0.0;
		velY = std::abs(velY) * bounceFactor;
		SetVelocity(velX, velY);
	}
	else if (posY + rect.h >= screenHeight)
	{
		posY = screenHeight - rect.h;
		velY = -std::abs(velY) * bounceFactor;
		SetVelocity(velX, velY);
	}

	rect.x = static_cast<int>(std::lround(posX));
	rect.y = static_cast<int>(std::lround(posY));
}

void Ball::OnCollision(GameObject& other)
{
	const SDL_Rect& otherRect = other.GetRect();
	double ballCenterX = posX + rect.w / 2.0;
	double ballCenterY = posY + rect.h / 2.0;
	double otherCenterX = otherRect.x + otherRect.w / 2.0;
	double otherCenterY = otherRect.y + otherRect.h / 2.0;

	double dx = ballCenterX - otherCenterX;
	double dy = ballCenterY - otherCenterY;
	double length = std::sqrt(dx * dx + dy * dy);

	constexpr double normalEpsilon = 1e-6;
	double normalX = 0.0;
	double normalY = 0.0;
	if (length > normalEpsilon)
	{
		normalX = dx / length;
		normalY = dy / length;
	}
	else
	{
		double penetrationX = (static_cast<double>(rect.w) + static_cast<double>(otherRect.w)) * 0.5 - std::abs(dx);
		double penetrationY = (static_cast<double>(rect.h) + static_cast<double>(otherRect.h)) * 0.5 - std::abs(dy);
		if (penetrationX <= penetrationY)
		{
			normalX = (ballCenterX >= otherCenterX) ? 1.0 : -1.0;
			normalY = 0.0;
		}
		else
		{
			normalX = 0.0;
			normalY = (ballCenterY >= otherCenterY) ? 1.0 : -1.0;
		}
		if (normalX == 0.0 && normalY == 0.0)
		{
			normalX = 1.0;
		}
	}

	double otherVelX = other.GetVelX();
	double otherVelY = other.GetVelY();

	if (!isActive)
	{
		isActive = true;
		SetVelocity(normalX * initialSpeed + otherVelX * 0.5,
		            normalY * initialSpeed + otherVelY * 0.5);
		return;
	}

	double currentVelX = GetVelX();
	double currentVelY = GetVelY();
	double currentSpeed = std::sqrt(currentVelX * currentVelX + currentVelY * currentVelY);
	if (currentSpeed < normalEpsilon)
	{
		currentSpeed = initialSpeed;
	}

	double targetSpeed = currentSpeed * bounceFactor;
	if (!other.IsStatic())
	{
		targetSpeed = std::max(initialSpeed, targetSpeed);
	}

	SetVelocity(normalX * targetSpeed + otherVelX * 0.5,
	            normalY * targetSpeed + otherVelY * 0.5);
}