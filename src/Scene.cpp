#include "Scene.hpp"

void Scene::Add(const GameObject& object)
{
	objects.push_back(std::make_unique<GameObject>(object));
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	objects.push_back(std::move(object));
}

void Scene::SetPlayer(GameObject* playerObject)
{
	player = playerObject;
}

GameObject* Scene::GetPlayer() const
{
	return player;
}

void Scene::TriggerGameOver(GameObject* killerEnemy)
{
	gameOver = true;
	gameOverEnemy = killerEnemy;
}

void Scene::Update(Uint32 deltaMs, const Uint8* keyboardState, int screenWidth, int screenHeight)
{
	if (gameOver)
	{
		return;
	}

	for (std::unique_ptr<GameObject>& object : objects)
	{
		object->Update(deltaMs, screenWidth, screenHeight, keyboardState);
	}
	RebuildBroadPhasePairs();
	RebuildNarrowPhasePairs();
	collisionResolver.Resolve(narrowPhasePairs);
	DispatchCollisionEvents();
}

void Scene::Render(SDL_Renderer* renderer) const
{
	if (gameOver && gameOverEnemy && gameOverEnemy->GetTexture())
	{
		int screenWidth = 0;
		int screenHeight = 0;
		SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
		
		SDL_Rect fullScreenRect = { 0, 0, screenWidth, screenHeight };
		SDL_RenderCopy(renderer, gameOverEnemy->GetTexture().get(), nullptr, &fullScreenRect);
		return;
	}

	for (const std::unique_ptr<GameObject>& object : objects)
	{
		object->Render(renderer);
	}
}

const std::vector<std::pair<GameObject*, GameObject*>>& Scene::GetBroadPhasePairs() const
{
	return broadPhasePairs;
}

const std::vector<std::pair<GameObject*, GameObject*>>& Scene::GetNarrowPhasePairs() const
{
	return narrowPhasePairs;
}

const std::vector<CollisionEvent>& Scene::GetCollisionEvents() const
{
	return collisionResolver.GetEvents();
}

void Scene::RebuildBroadPhasePairs()
{
	broadPhasePairs.clear();
	if (objects.size() < 2)
	{
		return;
	}

	struct SweepEntry
	{
		GameObject* object;
		int minX, maxX, minY, maxY;
	};

	std::vector<SweepEntry> entries;
	entries.reserve(objects.size());
	for (const std::unique_ptr<GameObject>& object : objects)
	{
		const SDL_Rect& rect = object->GetRect();
		entries.push_back(SweepEntry{ object.get(), rect.x, rect.x + rect.w, rect.y, rect.y + rect.h });
	}

	std::sort(entries.begin(), entries.end(), [](const SweepEntry& lhs, const SweepEntry& rhs)
	{
		return lhs.minX < rhs.minX;
	});

	std::vector<SweepEntry> active;
	active.reserve(entries.size());
	for (const SweepEntry& entry : entries)
	{
		active.erase(std::remove_if(active.begin(), active.end(), [&entry](const SweepEntry& candidate)
		{
			return candidate.maxX <= entry.minX;
		}), active.end());

		for (const SweepEntry& candidate : active)
		{
			if (!(candidate.maxY <= entry.minY || entry.maxY <= candidate.minY))
			{
				broadPhasePairs.emplace_back(candidate.object, entry.object);
			}
		}

		active.push_back(entry);
	}
}

void Scene::RebuildNarrowPhasePairs()
{
	narrowPhasePairs.clear();
	if (broadPhasePairs.empty())
	{
		return;
	}

	for (const std::pair<GameObject*, GameObject*>& pair : broadPhasePairs)
	{
		GameObject* lhs = pair.first;
		GameObject* rhs = pair.second;
		if (!lhs || !rhs)
		{
			continue;
		}

		const SDL_Rect& a = lhs->GetRect();
		const SDL_Rect& b = rhs->GetRect();
		if (!(a.x + a.w <= b.x || b.x + b.w <= a.x || a.y + a.h <= b.y || b.y + b.h <= a.y))
		{
			narrowPhasePairs.emplace_back(lhs, rhs);
		}
	}
}

void Scene::DispatchCollisionEvents()
{
	const std::vector<CollisionEvent>& events = collisionResolver.GetEvents();
	for (const CollisionEvent& event : events)
	{
		if (!event.first || !event.second)
		{
			continue;
		}

		if (event.type == CollisionEventType::Enter)
		{
			event.first->OnCollision(*event.second);
			event.second->OnCollision(*event.first);
		}
	}
}