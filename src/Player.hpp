#pragma once

#include "GameObject.hpp"
#include "Scene.hpp"
#include "Enemy.hpp"
#include <algorithm>

class Player : public GameObject
{
public:
	Player(int x, int y, int w, int h, Color color, double speed = 1.0f)
		: GameObject(x, y, w, h, color, speed) {}

	void Update(Uint32 deltaMs, int screenWidth, int screenHeight, const Uint8* keyboard = nullptr) override;
	void OnCollision(GameObject& other) override;
	void SetScene(Scene* sceneRef) { scene = sceneRef; }

private:
	Scene* scene = nullptr;
};