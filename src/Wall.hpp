#pragma once

#include "GameObject.hpp"

class Wall : public GameObject
{
public:
	Wall(int x, int y, int w, int h, Color color = Color(128, 128, 128, 255)) : GameObject(x, y, w, h, color, 0.0) {}

	void Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard = nullptr) override {}
	
	bool IsStatic() const override { return true; }

	void OnCollision(GameObject& other) override {}
};