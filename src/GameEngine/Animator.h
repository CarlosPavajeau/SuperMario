#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "GameObject.h"
#include "Pallete.h"
#include "SpriteSheet.h"

#include <SFML/Graphics.hpp>
#include <string>

namespace GameEngine
{
	class Animator : public GameObjects::GameObject
	{
	public:
		~Animator();

		void Create(const std::string& name, const sf::Texture& texture, const Vector& off_set, const Vector& size, int cols, int rows, float speed, AnimType anim_type = AnimType::ForwardCycle);
		void Create(const std::string& name, const sf::Texture& texture, const Rect& rect);
		void Create(const std::string& name, const sf::Texture& texture, const std::vector<sf::IntRect>& rects, float speed);

		void Play(const std::string& name);
		void Update(int delta_time) override;
		void Draw(sf::RenderWindow* wnd) override;

		void FlipX(bool value);

		void SetColor(const sf::Color& color);
		void SetSpeed(const std::string& animation, float speed);
		void SetSpriteOffset(const std::string& anim_name, int sprite_index, const Vector& value);
		void SetAnimOffset(float index);
		void SetPallete(Pallete* pallete);

		void Scale(float fX, float fY);
		SpriteSheet* Get(const std::string& str);

	private:
		Pallete* pallete = nullptr;
		std::unordered_map<std::string, SpriteSheet*> animations;
		SpriteSheet* current_animation = nullptr;
		std::string last_anim_name;
		bool flipped = false;
	};
}

#endif // !ANIMATOR_H
