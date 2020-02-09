#include "Mushroom.h"
#include "Mario.h"
#include "MarioGame.h"
#include "Blocks.h"
#include "GameEngine.h"

namespace GameObjects
{
	Mushroom::Mushroom(const Vector& pos, Creatures::Mario* mario)
	{
		as_flower = !mario->IsSmall();
		sprite.setTexture(*sMarioGame->TextureManager().Get("Items"));
		SetSize({ 31, 1 });
		SetPosition(pos.X, pos.Y + 32);
		this->mario = mario;

		if (!as_flower)
			sprite.setTextureRect({ 128, 150, 0, 0 });
		else
			sprite.setTextureRect({ 32, 212, 0, 0 });
	}

	void Mushroom::Draw(sf::RenderWindow* render_window)
	{
		sprite.setPosition(GetPosition());
		render_window->draw(sprite);
	}

	void Mushroom::Update(int delta_time)
	{
		timer += delta_time;

		if (GetPosition().Y > 1000)
			GetParent()->RemoveObject(this);

		switch (state)
		{
			case State::Wait:
			{
				if (timer > 400)
				{
					state = State::Borning;
					sMarioGame->PlaySound("powerup_appears");
					timer = 0;
				}

				break;
			}
			case State::Borning:
			{
				int height = timer * 0.02;

				if (height <= 32)
				{
					sprite.setTextureRect({ sprite.getTextureRect().left, sprite.getTextureRect().top, 32, height });
					Rect old_bounds = GetBounds();
					old_bounds.SetTop(old_bounds.Bottom() - height);
					SetBounds(old_bounds);
				}
				else
				{
					state = State::Normal;
					speed = Vector::Zero;
				}

				break;
			}
			case State::Normal:
			{
				if (!as_flower)
				{
					speed.X = run_speed;
					speed += Vector::Down * gravity_force * delta_time;
					Move(delta_time * speed);

					CollisionTag collision_tag = CollisionTag::None;
					SetPosition(blocks->CollisionResponse(GetBounds(), speed, delta_time, collision_tag));

					if (collision_tag & CollisionTag::Left || collision_tag & CollisionTag::Right)
						run_speed = -run_speed;
					if (collision_tag & CollisionTag::Floor || collision_tag & CollisionTag::Cell)
						speed.Y = 0;
				}
				else
				{
					int sprite_index = int(timer * 0.01) % 5;
					if (sprite_index > 2)
						sprite_index = 5 - sprite_index;
					sprite.setTextureRect({ 32 + sprite_index * 32, 212, 32, 32 });
				}

				if (mario && mario->GetBounds().IsIntersect(GetBounds()))
				{
					Action();
					GetParent()->RemoveObject(this);
				}
				break;
			}
			default:
				break;
		}
	}

	void Mushroom::AddImpulse(const Vector& speed)
	{
		this->speed += speed;
	}

	void Mushroom::Init()
	{
		blocks = GetParent()->FindObjectByType<Blocks>();
	}

	void Mushroom::Action()
	{
		mario->Promote();
		sMarioGame->AddScore(1000, GetBounds().Center());
		sMarioGame->PlaySound("powerup");
	}

	OneUpMushroom::OneUpMushroom(const Vector& pos, Creatures::Mario* mario) : Mushroom(pos, mario)
	{
		as_flower = false;
		sprite.setTextureRect({ 160, 150, 32, 0 });
	}

	void OneUpMushroom::Action()
	{
		sMarioGame->AddLive();
		sMarioGame->PlaySound("1-up");
	}
}