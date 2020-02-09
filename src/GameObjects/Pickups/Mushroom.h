#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "GameObject.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class Blocks;

	class Mushroom : public GameObject
	{
	public:
		Mushroom(const Vector& pos, Creatures::Mario* mario);

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		void AddImpulse(const Vector& speed);

	protected:
		sf::Sprite sprite;
		bool as_flower = false;

		void Init() override;
		virtual void Action();

	private:
		Creatures::Mario* mario;
		Blocks* blocks;
		Vector speed;

		enum class State
		{
			Wait,
			Borning,
			Normal
		} state = State::Wait;

		float timer = 0;
		float run_speed = 0.1f;
		const float gravity_force = 0.0015f;
	};

	class OneUpMushroom : public Mushroom
	{
	public:
		OneUpMushroom(const Vector& pos, Creatures::Mario* mario);

	private:
		virtual void Action() override;
	};

}
#endif // !MUSHROOM_H
