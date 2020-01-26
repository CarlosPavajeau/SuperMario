#include "GameObject.h"
#include "Animator.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class Coin : public GameObject
	{
	public:
		Coin();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		void Kick();

	protected:
		void OnActivated() override;
		void Init() override;

	private:
		enum class State
		{
			Twist,
			Shane
		} state = State::Twist;

		int remove_timer = 1000;
		GameEngine::Animator animator;
		Creatures::Mario* mario = nullptr;
	};
}