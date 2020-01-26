#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "GameObject.h"

namespace GameEngine
{
	enum class AnimType
	{
		Manual,
		Forward,
		ForwardStop,
		ForwardCycle,
		ForwardBackwardCycle
	};

	class SpriteSheet : public GameObjects::GameObject
	{
	public:
		SpriteSheet();

		void Load(const sf::Texture& texture, const std::vector<sf::IntRect>& rects);
		void Load(const sf::Texture& texture, const Vector& off_set, const Vector& size, int cols, int rows);

		void Draw(sf::RenderWindow* wnd) override;
		void Update(int delta_time) override;

		void SetAnimType(AnimType type);
		void SetSpriteIndex(int index);

		sf::Sprite* CurrentSprite() { return current_sprite; }
		sf::Vector2f GetPosition() const { return position; }
		AnimType GetAnimType() const { return anim_type; }

		void SetPosition(sf::Vector2f pos);
		void SetSpeed(float speed);
		void SetColor(const sf::Color& color);
		void SetRotation(float angle);
		void SetAnimOffset(float index);
		void SetOrigin(const Vector& vector);

		void Scale(float fX, float fY);
		void InvertH();
		void FlipX(bool isFliped);

		sf::Sprite& operator[](int index);

		void Reset();
		bool Empty() const;
		void Resize(const Vector& vector) { }

	private:
		AnimType anim_type;
		bool flipped = false;
		std::vector<sf::Sprite> sprites;
		sf::Sprite* current_sprite;
		float speed;
		sf::Vector2f position;
		sf::Vector2f torigin;
		float index = 0;
	};

}
#endif // !SPRITE_SHEET_H
