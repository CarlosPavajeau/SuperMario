#include "SpriteSheet.h"

namespace GameEngine
{
	SpriteSheet::SpriteSheet()
	{
		SetName("SpriteSheet");
		speed = 0.03f;
		current_sprite = nullptr;
		SetAnimType(AnimType::Manual);
		index = 0;
	}

	void SpriteSheet::Load(const sf::Texture& texture, const std::vector<sf::IntRect>& rects)
	{
		sprites.clear();
		for (auto& rect : rects)
			sprites.emplace_back(texture, rect);
		SetSpriteIndex(0);
	}

	void SpriteSheet::Load(const sf::Texture& texture, const Vector& off_set, const Vector& size, int cols, int rows)
	{
		sprites.clear();
		for (int y = 0; y < rows; ++y)
			for (int x = 0; x < cols; ++x)
				sprites.emplace_back(texture,
					sf::IntRect(x * std::abs(size.X) + off_set.X, y * 
								std::abs(size.Y) + off_set.Y, size.X, size.Y));
		SetSpriteIndex(0);
		SetAnimType(AnimType::Forward);
	}

	void SpriteSheet::Draw(sf::RenderWindow* wnd)
	{
		switch (anim_type)
		{
		case AnimType::Manual:
			break;
		case AnimType::Forward:
			int current_slide = int(index);
			if (current_slide < sprites.size())
				SetSpriteIndex(current_slide);
			else
				return;
			break;
		case AnimType::ForwardStop:
			int current_slide = int(index);
			if (current_slide < sprites.size())
				SetSpriteIndex(current_slide);
			break;
		case AnimType::ForwardCycle:
			int current_slide = int(index) % sprites.size();
			SetSpriteIndex(current_slide);
			break;
		case AnimType::ForwardBackwardCycle:
			int size = sprites.size();
			int current_slide = int(index) % (size * 2);
			if (current_slide > size - 1)
				current_slide = 2 * size - 1 - current_slide;
			SetSpriteIndex(current_slide);
			break;
		default:
			break;
		}

		wnd->draw(*current_sprite);
	}

	void SpriteSheet::Update(int delta_time)
	{
		if (IsEnabled() && speed)
			index += speed * delta_time;
	}

	void SpriteSheet::SetAnimType(AnimType type)
	{
		anim_type = type;
	}

	void SpriteSheet::SetSpriteIndex(int newIndex)
	{
		assert(newIndex >= 0 && newIndex < sprites.size());
		current_sprite = &sprites[newIndex];
	}

	void SpriteSheet::SetPosition(sf::Vector2f pos)
	{
		position = pos;
		for (auto& sprite : sprites)
			sprite.setPosition(position + torigin);
	}

	void SpriteSheet::SetSpeed(float value)
	{
		speed = value;
	}

	void SpriteSheet::SetColor(const sf::Color& color)
	{
		for (auto& sprite : sprites)
			sprite.setColor(color);
	}

	void SpriteSheet::SetRotation(float angle)
	{
		if (angle == sprites[0].getRotation())
			return;

		for (auto& sprite : sprites)
		{
			float width = sprite.getLocalBounds().width;
			float height = sprite.getLocalBounds().height;

			if (angle < 0)
				angle += 360;
			if (angle > 360)
				angle -= 360;

			if (angle == 0)
				sprite.setOrigin(0, 0);
			else if (angle == 90)
				sprite.setOrigin(0, height);
			else if (angle == 270)
				sprite.setOrigin(width, 0);
			else if (angle == 180)
				sprite.setOrigin(width, height);

			sprite.setRotation(angle);
		}
	}

	void SpriteSheet::SetAnimOffset(float i)
	{
		index += i;
	}

	void SpriteSheet::SetOrigin(const Vector& vector)
	{
		for (auto& sprite : sprites)
			sprite.setOrigin(vector);
	}

	void SpriteSheet::Scale(float fX, float fY)
	{
		for (auto& sprite : sprites)
			sprite.scale(fX, fY);
	}

	void SpriteSheet::InvertH()
	{

	}

	void SpriteSheet::FlipX(bool isFliped)
	{
		if (flipped != isFliped)
		{
			flipped = isFliped;

			for (auto& sprite : sprites)
			{
				auto rect = sprite.getTextureRect();
				rect.left += rect.width;
				rect.width = -rect.width;
				sprite.setTextureRect(rect);
			}
		}
	}

	sf::Sprite& SpriteSheet::operator[](int index)
	{
		assert(index >= 0 && index < sprites.size());
		return sprites[index];
	}

	void SpriteSheet::Reset()
	{
		index = 0;
	}

	bool SpriteSheet::Empty() const
	{
		return sprites.empty();
	}
}