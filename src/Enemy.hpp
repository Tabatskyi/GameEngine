#pragma once

#include "GameObject.hpp"
#include <algorithm>

class Player;

class Enemy : public GameObject
{
public:
	Enemy(int x, int y, int w, int h, Color color, double speed = 0.1);

	void Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard = nullptr) override;
	void SetTarget(GameObject* target);

private:
	GameObject* target = nullptr;
};