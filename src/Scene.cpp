#include "Scene.hpp"

void Scene::Add(const GameObject& object)
{
	objects.push_back(std::make_unique<GameObject>(object));
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	objects.push_back(std::move(object));
}

void Scene::Update(Uint32 deltaMs, const Uint8* keyboardState, int screenWidth, int screenHeight)
{
	for (std::unique_ptr<GameObject>& object : objects)
	{
		object->Update(deltaMs, screenWidth, screenHeight, keyboardState);
	}
}

void Scene::Render(SDL_Renderer* renderer) const
{
	for (const std::unique_ptr<GameObject>& object : objects)
	{
		object->Render(renderer);
	}
}