#include "Coin.h"
#include "MarioGame.h"
#include "Blocks.h"
#include "Mario.h"
#include "TwistedCoin.h"

namespace GameObjects
{
	Coin::Coin()
	{
		sf::Texture& texture = *sMarioGame->TextureManager().Get("Items");

		animator.Create("twist", texture, Vector(0, 84), Vector(32, 32), 4, 1, 0.01f);
		animator.Create("shine", texture, Vector(0, 116), Vector(40, 32), 5, 1, 0.01f, AnimType::Forward);
		animator.Get("shine")->SetOrigin(Vector(4, 0));
	}

	void Coin::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Coin::Update(int delta_time)
	{
		animator.Update(delta_time);

		if (state == State::Twist)
		{
			if (mario->GetBounds().IsIntersect(GetBounds()))
			{
				sMarioGame->AddScore(100);
				sMarioGame->AddCoin();
				sMarioGame->PlaySound("coin");

				animator.Play("shine");
				state = State::Shane;
			}
		}
		else if (state == State::Shane)
		{
			remove_timer -= delta_time;
			if (remove_timer < 0)
				GetParent()->RemoveObject(this);
		}
	}

	void Coin::Kick()
	{
		sMarioGame->AddScore(100);
		sMarioGame->AddCoin();
		sMarioGame->PlaySound("coin");

		GetParent()->AddObject(new TwistedCoin(GetPosition() + Vector::Up * 32, nullptr));
		GetParent()->RemoveObject(this);
	}

	void Coin::OnActivated()
	{
		SetSize({ GetProperty("Height").AsFloat(), GetProperty("Width").AsFloat() });
	}

	void Coin::Init()
	{
		mario = GetParent()->FindObjectByName<Creatures::Mario>("Mario");
	}
}