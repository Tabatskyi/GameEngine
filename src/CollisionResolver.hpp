#pragma once


#include <vector>
#include <utility>
#include <cstdint>
#include <algorithm>
#include <unordered_set>
#include "GameObject.hpp"

enum class CollisionEventType
{
	Enter,
	Exit
};

struct CollisionEvent
{
	GameObject* first;
	GameObject* second;
	CollisionEventType type;
};

class CollisionResolver
{
public:
	void Resolve(const std::vector<std::pair<GameObject*, GameObject*>>& overlappingPairs);
	const std::vector<CollisionEvent>& GetEvents() const { return events; }

private:
	struct PairKey
	{
		GameObject* first;
		GameObject* second;

		bool operator==(const PairKey& other) const noexcept
		{
			return first == other.first && second == other.second;
		}
	};

	struct PairKeyHasher
	{
		size_t operator()(const PairKey& key) const noexcept
		{
			return std::hash<GameObject*>()(key.first) ^ (std::hash<GameObject*>()(key.second) << 1);
		}
	};

	static PairKey Normalize(GameObject* first, GameObject* second);
	static void ResolvePair(GameObject* first, GameObject* second);

	std::unordered_set<PairKey, PairKeyHasher> activePairs;
	std::vector<CollisionEvent> events;
};