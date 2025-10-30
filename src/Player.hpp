#pragma once

#include "GameObject.hpp"

class Player : public GameObject
{
public:
	Player(int x, int y, int w, int h, Color color, float speed = 1.0f)
		: GameObject(x, y, w, h, color, speed) {}

	void Update(Uint32 deltaMs, const Uint8* keyboardState, int screenWidth, int screenHeight) override;
};