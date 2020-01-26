#ifndef START_H
#define START_H

#include "GameObject.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class Blocks;

	class Start : public GameObject
	{
	public:
		Start(const Vector& pos, Creatures::Mario* mario);

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		enum State
		{
			Wait,
			Borning,
			Normal
		} state = State::Wait;

		sf::Sprite sprite;
		float timer = 0;
		Creatures::Mario* mario = nullptr;
		Vector speed;
		Blocks* blocks;

		const float gravity_force = 0.0015f;
		const float jump_power = 0.6f;
		const float horizontal_speed = 0.1f;

		void Init() override;
	};
}

#endif // !START_H