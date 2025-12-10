#include "Scene.hpp"
#include <algorithm>

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
	RebuildBroadPhasePairs();
	RebuildNarrowPhasePairs();
}

void Scene::Render(SDL_Renderer* renderer) const
{
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

void Scene::RebuildBroadPhasePairs()
{
	broadPhasePairs.clear();
	if (objects.size() < 2)
	{
		narrowPhasePairs.clear();
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
		for (const SweepEntry& candidate : active)
		{
			if (candidate.maxX <= entry.minX) 
			{
				active.erase(std::remove(active.begin(), active.end(), candidate), active.end());
			}
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