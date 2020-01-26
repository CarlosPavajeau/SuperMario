#ifndef JUMPER_H
#define JUMPER_H

#include "Item.h"
#include "SpriteSheet.h"

namespace GameObjects
{
	class Jumper : public Item
	{
	public:
		Jumper();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

		virtual void CollisionResponse(Creatures::Mario* mario, CollisionTag& collisionTag, int delta_time) override;

	private:
		SpriteSheet* sprite_sheet;
		Creatures::Mario* mario;
		bool zipped;
		float buttom;
		int state;
		float timer;

		void OnActivated() override;
	};
}

#endif // !JUMPER_H
