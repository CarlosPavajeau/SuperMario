#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"
#include "GameEngine.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	using namespace GameEngine;

	enum class PlatformType
	{
		Vertical,
		Horizontal,
		Elevator,
		Skate,
		NoInit = 100
	};

	class Item : public GameObject
	{
	public:
		virtual void CollisionResponse(Creatures::Mario* mario, CollisionTag& collisionTag, int delta_time) = 0;
	};
}

#endif // !ITEM_H