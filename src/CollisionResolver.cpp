#include "CollisionResolver.hpp"

void CollisionResolver::Resolve(const std::vector<std::pair<GameObject*, GameObject*>>& overlappingPairs)
{
	events.clear();
	std::unordered_set<PairKey, PairKeyHasher> currentPairs;
	currentPairs.reserve(overlappingPairs.size());

	for (const std::pair<GameObject*, GameObject*>& pair : overlappingPairs)
	{
		GameObject* first = pair.first;
		GameObject* second = pair.second;
		if (!first || !second)
		{
			continue;
		}

		PairKey key = Normalize(first, second);
		ResolvePair(first, second);

		if (!activePairs.contains(key))
		{
			events.push_back(CollisionEvent{ first, second, CollisionEventType::Enter });
		}

		currentPairs.insert(key);
	}

	for (const PairKey& key : activePairs)
	{
		if (!currentPairs.contains(key))
		{
			events.push_back(CollisionEvent{ key.first, key.second, CollisionEventType::Exit });
		}
	}

	activePairs = std::move(currentPairs);
}

CollisionResolver::PairKey CollisionResolver::Normalize(GameObject* first, GameObject* second)
{
	if (!first || !second)
	{
		return PairKey{ first, second };
	}

	std::uintptr_t addrFirst = reinterpret_cast<std::uintptr_t>(first);
	std::uintptr_t addrSecond = reinterpret_cast<std::uintptr_t>(second);
	if (addrFirst <= addrSecond)
	{
		return PairKey{ first, second };
	}
	return PairKey{ second, first };
}

void CollisionResolver::ResolvePair(GameObject* first, GameObject* second)
{
	if (!first || !second)
	{
		return;
	}

	const SDL_Rect& a = first->GetRect();
	const SDL_Rect& b = second->GetRect();

	int overlapLeft = std::max(a.x, b.x);
	int overlapRight = std::min(a.x + a.w, b.x + b.w);
	int overlapTop = std::max(a.y, b.y);
	int overlapBottom = std::min(a.y + a.h, b.y + b.h);

	int overlapX = overlapRight - overlapLeft;
	int overlapY = overlapBottom - overlapTop;

	if (overlapX <= 0 || overlapY <= 0)
	{
		return;
	}

	double pushX = static_cast<double>(overlapX) / 2.0;
	double pushY = static_cast<double>(overlapY) / 2.0;

	if (overlapX < overlapY)
	{
		if (a.x < b.x)
		{
			first->OffsetPosition(-pushX, 0.0);
			second->OffsetPosition(pushX, 0.0);
		}
		else
		{
			first->OffsetPosition(pushX, 0.0);
			second->OffsetPosition(-pushX, 0.0);
		}

		first->SetVelocity(-first->GetVelX(), first->GetVelY());
		second->SetVelocity(-second->GetVelX(), second->GetVelY());
	}
	else
	{
		if (a.y < b.y)
		{
			first->OffsetPosition(0.0, -pushY);
			second->OffsetPosition(0.0, pushY);
		}
		else
		{
			first->OffsetPosition(0.0, pushY);
			second->OffsetPosition(0.0, -pushY);
		}

		first->SetVelocity(first->GetVelX(), -first->GetVelY());
		second->SetVelocity(second->GetVelX(), -second->GetVelY());
	}
}