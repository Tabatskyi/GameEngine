#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include "GameObject.hpp"
#include "CollisionResolver.hpp"

class Scene
{
public:
	void Add(const GameObject& object);
	void Add(std::unique_ptr<GameObject> object);
	void Update(Uint32 deltaMs, const Uint8* keyboardState, int screenWidth, int screenHeight);
	void Render(SDL_Renderer* renderer) const;
	const std::vector<std::pair<GameObject*, GameObject*>>& GetBroadPhasePairs() const;
	const std::vector<std::pair<GameObject*, GameObject*>>& GetNarrowPhasePairs() const;
	const std::vector<CollisionEvent>& GetCollisionEvents() const;
	
	void SetPlayer(GameObject* playerObject);
	GameObject* GetPlayer() const;

private:
	void RebuildBroadPhasePairs();
	void RebuildNarrowPhasePairs();
	void DispatchCollisionEvents();
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::pair<GameObject*, GameObject*>> broadPhasePairs;
	std::vector<std::pair<GameObject*, GameObject*>> narrowPhasePairs;
	CollisionResolver collisionResolver;
	GameObject* player = nullptr;
};