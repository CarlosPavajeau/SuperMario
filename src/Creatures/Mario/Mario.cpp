#include "Mario.h"
#include "MarioGame.h"
#include "MarioState.h"
#include "MarioBullet.h"
#include "Item.h"
#include "Enemy.h"
#include "Animator.h"
#include "Ladder.h"
#include "Blocks.h"

using namespace GameObjects;

namespace Creatures
{
	Mario::Mario()
	{
		blocks = nullptr;

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Mario");

		AddObject(animator = new Animator());

		animator->Create("idle_big", texture, { 0, 32, 32, 64 });
		animator->Create("walk_big", texture, { 32, 32 }, { 32, 64 }, 3, 1, 0.01f);
		animator->Create("swim_big", texture, { 321, 32 }, { 40, 64 }, 3, 1, 0.01f);
		animator->Create("jump_big", texture, { 160,32,32,64 });
		animator->Create("slip_big", texture, { 128,32,32,64 });
		animator->Create("seat_big", texture, { 192,52,32,44 });
		animator->Create("climb_big", texture, Vector(256, 32), Vector(32, 64), 2, 1, 0.01f);
		animator->Create("growing", texture, { { 0,32,32,64 }, { 0,96,32,32 } }, 0.01f);
		animator->SetSpriteOffset("growing", 1, { 0, 32 });
		animator->Create("demoting", texture, { { 401,32,40,64 },{ 288,96,32,32 } }, 0.01f);
		animator->SetSpriteOffset("demoting", 1, { 4, 32 });
		animator->Create("firing", texture, { { 0,32,32,64 },{ 224,32,32,64 } }, 0.01f);
		animator->Create("idle_small", texture, { 0,96,32,32 });
		animator->Create("walk_small", texture, { 32, 96 }, { 32, 32 }, 3, 1, 0.01f);
		animator->Create("swim_small", texture, { 288, 96 }, { 32, 32 }, 3, 1, 0.01f);
		animator->Create("jump_small", texture, { 160,96,32,32 });
		animator->Create("slip_small", texture, { 128,96,32,32 });
		animator->Create("seat_small", texture, { 192,96,32,32 });
		animator->Create("climb_small", texture, Vector(224, 96), Vector(32, 32), 2, 1, 0.01f);
		animator->Create("shoot", texture, { 224,32,32,64 });
		animator->Create("died", texture, { 192,96,32,32 });

		SetRank(MarioRank::Small);
		SetState(MarioState::Normal);

		fire_pallete.Create({ sf::Color(64,64,128), sf::Color(64,96,192), sf::Color(160,32,0), sf::Color(192,0,64), sf::Color(224,32,64) },
			{ sf::Color(64,128,0),sf::Color(96,160,0),sf::Color(192,192,128),sf::Color(224,224,128), sf::Color(255,251,240) });

		black_pallete.Create({ sf::Color(64,64,128), sf::Color(64,96,192), sf::Color(160,32,0), sf::Color(192,0,64), sf::Color(224,32,64) },
			{ sf::Color(128,64,0),sf::Color(160,96,0),sf::Color(20,20,10),sf::Color(30,30,20), sf::Color(0,0,0) });

		animator->Play("idle_small");
		animator->Get("seat_big")->SetOrigin(-Vector::Up * 12);
	}

	void Mario::SetRank(MarioRank rank)
	{
		switch (rank)
		{
			case MarioRank::Small:
			{
				SetSize(small_mario_size);

				if (this->rank != MarioRank::Small)
					Move(small_mario_size - big_mario_size);

				seated = false;
				animator->SetPallete(nullptr);
				animator->Play("idle_small");

				break;
			}
			case MarioRank::Big:
			{
				SetSize(big_mario_size);
				Move(small_mario_size - big_mario_size);
				animator->SetPallete(nullptr);
				animator->Play("idle_big");

				break;
			}
			case MarioRank::Fire:
			{
				animator->SetPallete(&fire_pallete);

				if (!seated)
				{
					SetSize(big_mario_size);
					animator->Play("idle_big");
				}
			}
		}

		this->rank = rank;
	}

	void Mario::Promote()
	{
		if (rank != MarioRank::Fire)
			SetState(MarioState::Promoting);
	}

	void Mario::ReciveDamage()
	{
		if (!spawn_timer && !invicible_mode)
		{
			if (rank != MarioRank::Small)
				SetState(MarioState::Demoting);
			else
				SetState(MarioState::Died);
		}
	}

	bool Mario::IsSmall() const
	{
		return GetRank() == MarioRank::Small;
	}

	void Mario::SetUnclimb()
	{
		climb = false;
		jumped = false;
		grounded = false;
		speed = Vector::Down * 0.2f;
		PlayAnimation("jump_" + std::string(IsSmall() ? "small" : "big"));
	}

	void Mario::SetState(MarioState state)
	{
		if (current_state)
		{
			current_state->OnLeave();
			delete current_state;
		}

		switch (state)
		{
		case MarioState::Normal:
			current_state = new NormalMarioState();
			break;
		case MarioState::Promoting:
			current_state = new PromotingMarioState();
			break;
		case MarioState::Demoting:
			current_state = new DemotingMarioState();
			break;
		case MarioState::Died:
			current_state = new DiedMarioState();
			break;
		default:
			break;
		}

		current_state->SetMario(this);
		current_state->OnEnter();
	}

	void Mario::SetState(IMarioState* state)
	{
		if (current_state)
		{
			current_state->OnLeave();
			delete current_state;
		}

		current_state = state;
		current_state->SetMario(this);
		current_state->OnEnter();
	}

	void Mario::StandUp()
	{
		if (seated)
		{
			SetSize(big_mario_size);
			Move(seated_mario_size - big_mario_size);
			seated = false;
		}
	}

	void Mario::SeatDown()
	{
		if (!seated)
		{
			SetSize(seated_mario_size);
			Move(big_mario_size - seated_mario_size);
			seated = true;
		}
	}

	void Mario::Fire()
	{
		Vector pos = GetBounds().Center + direction * 25 + Vector::Up * 8;
		GetParent()->AddObject(new MarioBullet(pos, direction));
		sMarioGame->PlaySound("fireball");
	}

	void Mario::Draw(sf::RenderWindow* render_window)
	{
		animator->SetPosition(GetPosition());
		animator->Draw(render_window);
	}

	void Mario::Update(int delta_time)
	{
		if (!blocks)
			blocks = GetParent()->FindObjectByType<Blocks>();
		assert(blocks);

		current_state->Update(delta_time);
		GameObject::Update(delta_time);
	}

	void Mario::InputProcessing(float delta_time)
	{
		auto& input_manager = sMarioGame->InputManager();

		input_direction = input_manager.GetXYAxis();

		if (!climb)
		{
			x_max_speed = (input_manager.IsButtonPressed("Fire") && 
							!jumped && !speed.Y) ? run_speed : walk_speed;

			if (!seated || jumped)
			{
				float k = 1.f;
				if (jumped)
					k = 0.7f;

				if ((input_direction.X < 0) && !(collision_tag & CollisionTag::Left))
					AddImpulse(Vector::Left * (acceleration * delta_time * k));
				if ((input_direction.X > 0) && !(collision_tag & CollisionTag::Right))
					AddImpulse(Vector::Right * (acceleration * delta_time * k));

				if (grounded && input_direction.X)
					direction.X = input_direction.X;
			}

			if (grounded && rank != MarioRank::Small)
			{
				if (input_direction.Y > 0 && !seated)
					SeatDown();
				else if (input_direction.Y <= 0 && seated)
					StandUp();
			}

			if (grounded)
				jumped = false;

			if (input_manager.IsButtonDown("Fire") && seated && fire_timer > fire_rate&& rank == MarioRank::Fire)
			{
				Fire();
				fire_timer = 0;
			}
			fire_timer += delta_time;
		}
		else
		{
			if (!input_direction.X)
			{
				if (input_direction == Vector::Up)
				{
					if (GetPosition().Y >= used_ladder->GetPosition().Y)
						Move(climb_speed * Vector::Up * delta_time);
				}
				else if (input_direction == Vector::Down)
				{
					if (GetBounds().Buttom <= used_ladder->GetBounds().Buttom)
						Move(climb_speed * Vector::Down * delta_time);
				}
			}
			else
			{
				AddImpulse({ run_speed * input_direction.X, -0.3f });
				climb = false;
			}

			if (collision_tag & CollisionTag::Floor)
				climb = false;
		}

		jumping_timer += delta_time;

		if (jump_timer)
		{
			jump_timer -= delta_time;
			if (jump_timer < 0)
				jump_timer = 0;
		}

		if (!in_water)
		{
			if (input_manager.IsButtonPressed("Jump") && !(collision_tag & CollisionTag::Cell))
			{
				if (grounded && !jump_timer)
				{
					sMarioGame->PlaySound("jump_super");
					grounded = climb = false;
					jumped = true;
					jumping_timer = 0.65f * jump_time;
					AddImpulse(1.5 * Vector::Up * jump_force);
					jump_timer = jump_rate;
				}
				else if (jumping_timer < jump_time)
					AddImpulse(Vector::Up * jump_force * (1 - jumping_timer / jump_time));
			}
			jumping_timer = jump_time;
		}
		else
		{
			if (input_manager.IsButtonPressed("Jump"))
			{
				if (grounded)
					jumped = true;

				if (jumping_timer > 100)
				{
					AddImpulse(0.5 * Vector::Up * jump_force);
					jumping_timer = 0;
				}
			}
		}

		if (input_manager.IsButtonPressed("Jump") && climb && !input_direction.Y)
		{
			AddImpulse(0.4f * Vector::Up * jump_force);
			grounded = climb = false;
			jumped = true;
		}
	}

	void Mario::PhysicsProccesing(float delta_time)
	{
		if (climb)
			return;

		if (!in_water)
		{
			if (grounded)
				AddImpulse(-Vector::Right * math::sign(speed.X) * friction_force * delta_time);
			AddImpulse(Vector::Down * gravity_force * delta_time);

			if (std::abs(speed.X) < 0.01f)
				speed.X = 0.f;
			speed.X = math::clamp(speed.X, -x_max_speed, x_max_speed);
			Move(speed * delta_time);
		}
		else
		{
			if (grounded)
				AddImpulse(-Vector::Right * math::sign(speed.X) * friction_force * delta_time);
			AddImpulse(0.25 * Vector::Down * gravity_force * delta_time);

			if (std::abs(speed.X) < 0.01)
				speed.X = 0;
			speed.X = math::clamp(speed.X, -x_max_speed / 2, x_max_speed / 2);
			Move(speed * delta_time);
			if (GetPosition().Y < 0)
				speed.Y = 0.1f;
		}
	}

	void Mario::CollisionProcessing(float delta_time)
	{
		collision_tag = CollisionTag::None;

		SetPosition(blocks->CollisionResponse(GetBounds(), speed, delta_time, collision_tag));

		auto items = GetParent()->FindObjectsByType<Item>();

		for (auto item : items)
			if (item->GetBounds().IsIntersect(GetBounds()))
				item->CollisionResponse(this, collision_tag, delta_time);

		if (!climb && input_direction.Y)
		{
			auto ladders = GetParent()->FindObjectsByType<Ladder>();
			for (auto ladder : ladders)
			{
				if (ladder->GetBounds().IsIntersect(GetBounds()))
				{
					used_ladder = ladder;
					climb = true;
					jumped = true;

					int x_ladder = ladder->GetBounds().Center.X;
					if (x_ladder > GetBounds().Center.X)
					{
						SetPosition(x_ladder - GetBounds().Width, GetPosition().Y);
						animator->FlipX(false);
					}
					else
					{
						SetPosition(x_ladder, GetPosition().Y);
						animator->FlipX(true);
					}
					speed = Vector::Zero;
					break;
				}
			}
		}

		grounded = collision_tag & CollisionTag::Floor;

		if ((collision_tag & CollisionTag::Right) && input_direction != Vector::Left)
			speed.X = 0;
		if ((collision_tag & CollisionTag::Left) && input_direction != Vector::Right)
			speed.X = 0;

		if (collision_tag & CollisionTag::Cell)
			speed.Y = 0.1f;
		if (collision_tag & CollisionTag::Floor)
			speed.Y = 0.f;
	}

	void Mario::KickBlocksProcessing()
	{
		if (collision_tag & CollisionTag::Cell && !(collision_tag & CollisionTag::Floor))
		{
			Vector block_left = blocks->ToBlockCoordinates(GetBounds().LeftTop + Vector::Up * 16);
			Vector block_right = blocks->ToBlockCoordinates(GetBounds().RightTop + Vector::Up * 16);

			bool block_left_exist = blocks->IsBlockInBounds(block_left) && (blocks->IsCollidableBlock(block_left) || blocks->IsInvizibleBlock(block_left));
			bool block_right_exist = blocks->IsBlockInBounds(block_right) && (blocks->IsCollidableBlock(block_right) || blocks->IsInvizibleBlock(block_right));

			if (block_left_exist || block_right_exist)
			{
				if (block_left_exist ^ block_right_exist)
				{
					if (block_left_exist)
						blocks->KickBlock(block_left.X, block_left.Y, this);
					else
						blocks->KickBlock(block_right.X, block_right.Y, this);
				}
				else
				{
					Rect block_left_rect = blocks->GetBlockBounds(block_left);
					Rect block_right_rect = blocks->GetBlockBounds(block_right);

					float mario_x = GetBounds().Center.X;

					if (std::abs(block_left_rect.Center.X - mario_x) < std::abs(block_right_rect.Center.X - mario_x))
						blocks->KickBlock(block_left.X, block_left.Y, this);
					else
						blocks->KickBlock(block_right.X, block_right.Y, this);
				}
			}
		}
	}

	void Mario::KickEnemiesProcessing(float delta_time)
	{
		if (spawn_timer == 0)
		{
			auto enemies = GetParent()->FindObjectsByType<Enemy>();

			for (auto enemy : enemies)
			{
				if (enemy->IsAlive() && GetBounds().IsIntersect(enemy->GetBounds()))
				{
					if (invicible_mode)
						enemy->KickFromBottom(this);
					else
					{
						if (!in_water)
						{
							Rect intersection = GetBounds().GetIntersection(enemy->GetBounds());
							if (intersection.Height / 8 < intersection.Width &&
								intersection.Buttom == GetBounds().Buttom)
							{
								enemy->KickFromTop(this);
								Move(-speed * delta_time);
								speed.Y = -0.3f;
							}
							else
								enemy->TouchSide(this);
						}
						else enemy->TouchSide(this);
					}
				}
			}
		}
	}

	void Mario::AnimationProcessing(float delta_time)
	{
		std::string rank_string = (rank == MarioRank::Small) ? "_small" : "_big";

		if (climb)
		{
			PlayAnimation("climb" + rank_string);
			animator->SetSpeed("climb" + rank_string, input_direction.Y ? 0.005f : 0.f);
		}
		else if (in_water)
		{
			if (!speed.Y)
			{
				if (speed.X)
				{
					PlayAnimation("walk" + rank_string);
					animator->FlipX(speed.X < 0);
					animator->SetSpeed("walk" + rank_string, 0.02f * (std::abs(speed.X) / walk_speed));

					if (direction == Vector::Left && speed.X > 0 ||
						direction == Vector::Right && speed.X < 0)
						PlayAnimation("idle" + rank_string);
				}
				else
					PlayAnimation("idle" + rank_string);
			}
			else
			{
				PlayAnimation("swim" + rank_string);
				animator->FlipX(speed.X < 0);
				animator->SetSpeed("swing" + rank_string, 0.02f * (std::abs(speed.X) / walk_speed));
			}
		}
		else
		{
			if (GetRank() == MarioRank::Fire && fire_timer < 200 && seated)
				PlayAnimation("shoot");
			else if (seated)
				PlayAnimation("seat" + rank_string);
			else if (jumped)
				PlayAnimation("jump" + rank_string);
			else if (grounded)
			{
				if (speed.X)
				{
					PlayAnimation("walk" + rank_string);
					animator->FlipX(speed.X < 0);
					animator->SetSpeed("walk" + rank_string, 0.02f * (std::abs(speed.X) / walk_speed));

					if (direction == Vector::Left && speed.X > 0 ||
						direction == Vector::Right && speed.X < 0)
						PlayAnimation("slip" + rank_string);
				}
				else
					PlayAnimation("idle" + rank_string);
			}
			else
				animator->SetSpeed("walk" + rank_string, 0.f);
		}

		if (spawn_timer)
		{
			spawn_timer -= delta_time;

			if (int(spawn_timer / 80) % 2)
				animator->SetColor(sf::Color::White);
			else
				animator->SetColor(sf::Color::Transparent);

			if (spawn_timer < 0)
			{
				animator->SetColor(sf::Color::White);
				spawn_timer = 0;
			}
		}
	}

	void Mario::PlayAnimation(const std::string& name)
	{
		animator->Play(name);
	}

	void Mario::OnActivated()
	{
		if (GetProperty("InWater").IsValid() && GetProperty("InWater").AsBool())
			in_water = true;
	}

	void Mario::Init()
	{
		if (GetProperty("SpawnDirection").IsValid())
		{
			Vector direction = ToVector(GetProperty("SpawnDirection").AsString());
		}
	}
}