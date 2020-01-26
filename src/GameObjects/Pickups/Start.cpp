#include "Start.h"
#include "Mario.h"
#include "Blocks.h"
#include "MarioGame.h"

using namespace Creatures;

namespace GameObjects
{
	Start::Start(const Vector& pos, Mario* mario)
	{
		sprite.setTexture(*sMarioGame->TextureManager().Get("Items"));
		sprite.setTextureRect({ 0, 0, 0, 0 });
		SetSize({ 31, 1 });
		SetPosition(pos.X, pos.Y + 32);
		this->mario = mario;
		speed.Y = -jump_power;
		speed.X = horizontal_speed;
	}

	void Start::Draw(sf::RenderWindow* render_window)
	{
		sprite.setPosition(GetPosition());
		render_window->draw(sprite);
	}

	void Start::Update(int delta_time)
	{
		timer += delta_time;

		int sprite_index = int(timer * 0.01) % 5;
		if (sprite_index > 2)
			sprite_index = 5 - sprite_index;

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
					sprite.setTextureRect({ 128 + sprite_index * 32 ,212,32,height });
					Rect old_bounds = GetBounds();
					old_bounds.SetTop(old_bounds.Buttom - height);
					SetBounds(old_bounds);
				}
				else
					state = State::Normal;

				break;
			}
			case State::Normal:
			{
				sprite.setTextureRect({ 128 + sprite_index * 32, 212, 32, 32 });

				CollisionTag collision_tag = CollisionTag::None;
				blocks->CollisionResponse(GetBounds(), speed, delta_time, collision_tag);

				if (collision_tag & CollisionTag::Floor)
					speed.Y = -jump_power;

				if (collision_tag & CollisionTag::Right || collision_tag & CollisionTag::Left)
					speed.X = -speed.X;

				speed += Vector::Down * gravity_force * delta_time;
				Move(delta_time * speed);

				if (GetPosition().Y > 1000)
					GetParent()->RemoveObject(this);

				if (mario && mario->GetBounds().IsIntersect(GetBounds()))
				{
					GetParent()->RemoveObject(this);
					sMarioGame->InvicibleMode();
				}

				break;
			}
			default:
				break;
		}
	}

	void Start::Init()
	{
		blocks = GetParent()->FindObjectByType<Blocks>();
	}
}