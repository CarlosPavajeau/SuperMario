#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
#include "GameEngine.h"
#include "Pallete.h"

namespace GameObjects
{
	class Blocks;
	class Ladder;
}

namespace GameEngine
{
	class Animator;
}

using namespace GameEngine;

namespace Creatures
{
	enum class MarioRank;
	enum class MarioState;

	class IMarioState;

	class Mario : public GameObjects::GameObject
	{
	public:
		Mario();

		MarioRank GetRank() const { return rank; }
		void SetRank(MarioRank rank);
		void Promote();

		Vector GetSpeed() const { return speed; }
		void SetSpeed(const Vector& speed) { this->speed = speed; }
		void AddImpulse(Vector speed) { this->speed += speed; }
		void ReciveDamage();

		Vector GetInputDirection() { return input_direction; }

		bool IsGrounded() const { return grounded; }
		bool IsClimbing() const { return climb; }
		bool IsSmall() const;

		void SetUnclimb();
		void SetState(MarioState state);
		void SetState(IMarioState* state);
		void SetInvicibleMode(bool value = true) { invicible_mode = value; }

		void StandUp();
		void SeatDown();

		bool IsSeated() const { return seated; }

	private:
		const float jump_force = 0.3f;
		const float jump_time = 300.f;
		const float walk_speed = 0.2f;
		const float run_speed = 0.3f;
		const float acceleration = 0.001f;
		const float friction_force = 0.0005f;
		const float gravity_force = 0.0015f;
		const float climb_speed = 0.1f;
		const int invincible_time = 11000;
		const int fire_rate = 400; //bullet per ms
		const float jump_rate = 250.f; //jump per ms
		const Vector small_mario_size = { 31,32 };
		const Vector big_mario_size = { 31,64 };
		const Vector seated_mario_size = { 31,32 };

		float jumping_timer = 0;
		float jump_timer = jump_rate;

		MarioRank rank;
		MarioState state;
		IMarioState* current_state = nullptr;
		CollisionTag collision_tag;

		bool invicible_mode = false;
		int invincible_timer = invincible_time;

		Vector speed;
		float x_max_speed;

		bool grounded = false;
		bool jumped = false;
		bool seated = false;
		bool climb = false;
		bool in_water = false;

		GameObjects::Ladder* used_ladder = nullptr;
		GameObjects::Blocks* blocks;
		Animator* animator;

		float fire_timer = 0;
		float spawn_timer = 0;

		Vector direction = Vector::Right;
		Vector input_direction;

		Pallete fire_pallete, black_pallete;

		void Fire();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

		void SetGrounded(bool value = true) { grounded = value; }

		void InputProcessing(float delta_time);
		void PhysicsProccesing(float delta_time);
		void CollisionProcessing(float delta_time);
		void KickBlocksProcessing();
		void KickEnemiesProcessing(float delta_time);
		void AnimationProcessing(float delta_time);

		void PlayAnimation(const std::string& name);

		void OnActivated() override;
		void Init() override;

		friend class IMarioState;
		friend class PromotingMarioState;
		friend class DemotingMarioState;
		friend class NormalMarioState;
		friend class TransitionMarioState;
		friend class GoToCastleMarioState;
		friend class GoToPortalState;
		friend class GoToPrincessState;
		friend class DiedMarioState;
	};
}

#endif // !MARIO_H
