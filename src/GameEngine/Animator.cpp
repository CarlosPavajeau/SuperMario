#include "Animator.h"

namespace GameEngine
{
	Animator::~Animator()
	{
		for (auto anim : animations)
			delete anim.second;
	}

	void Animator::Create(const std::string& name, const sf::Texture& texture, const Vector& off_set, const Vector& size, int cols, int rows, float speed, AnimType anim_type = AnimType::ForwardCycle)
	{
		SpriteSheet* animation = new SpriteSheet();

		animation->Load(texture, off_set, size, cols, rows);
		animation->SetAnimType(anim_type);
		animation->SetSpeed(speed);

		assert(!animation[name]);

		animations[name] = animation;

		if (!current_animation) current_animation = animation;
	}

	void Animator::Create(const std::string& name, const sf::Texture& texture, const Rect& rect)
	{
		SpriteSheet* animation = new SpriteSheet();

		animation->Load(texture, { { (int)rect.Left, (int)rect.Top, (int)rect.Width, (int)rect.Height } });

		assert(!animation[name]);

		animations[name] = animation;

		if (!current_animation) current_animation = animation;
	}

	void Animator::Create(const std::string& name, const sf::Texture& texture, const std::vector<sf::IntRect>& rects, float speed)
	{
		SpriteSheet* animation = new SpriteSheet();

		animation->Load(texture, rects);
		animation->SetAnimType(AnimType::ForwardCycle);
		animation->SetSpeed(speed);

		assert(!animation[name]);

		animations[name] = animation;

		if (!current_animation) current_animation = animation;
	}

	void Animator::Play(const std::string& name)
	{
		if (last_anim_name != name)
		{
			current_animation = animations[name];

			assert(current_animation);

			last_anim_name = name;
			current_animation->Reset();
		}
	}

	void Animator::Update(int delta_time)
	{
		if (IsEnabled())
			current_animation->Update(delta_time);
	}

	void Animator::Draw(sf::RenderWindow* wnd)
	{
		if (IsVisible())
		{
			if (pallete)
				pallete->Apply();

			current_animation->SetPosition(GetPosition());
			current_animation->Draw(wnd);

			if (pallete)
				pallete->Cancel();
		}
	}

	void Animator::FlipX(bool value)
	{
		if (value != flipped)
		{
			flipped = value;

			for (auto& animation : animations)
				animation.second->FlipX(value);
		}
	}

	void Animator::SetColor(const sf::Color& color)
	{
		for (auto& animation : animations)
			animation.second->SetColor(color);
	}

	void Animator::SetSpeed(const std::string& animation, float speed)
	{
		animations[animation]->SetSpeed(speed);
	}

	void Animator::SetSpriteOffset(const std::string& anim_name, int sprite_index, const Vector& value)
	{
		SpriteSheet* sheet = animations[anim_name];

		assert(sheet);

		sheet->operator[](sprite_index).setOrigin(-value);
	}

	void Animator::SetAnimOffset(float index)
	{
		for (auto& animation : animations)
			animation.second->SetAnimOffset(index);
	}

	void Animator::SetPallete(Pallete* newPallete)
	{
		pallete = newPallete;
	}

	void Animator::Scale(float fX, float fY)
	{
		for (auto& animation : animations)
			animation.second->Scale(fX, fY);
	}

	SpriteSheet* Animator::Get(const std::string& str)
	{
		return animations[str];
	}
}