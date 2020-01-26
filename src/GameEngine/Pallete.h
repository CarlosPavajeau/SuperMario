#ifndef PALLETE_H
#define PALLETE_H

#include <list>
#include <SFML/Graphics.hpp>

namespace GameEngine
{
	class Pallete
	{
	public:
		Pallete();

		void Create(const std::initializer_list<sf::Color>& original_colors, const std::initializer_list<sf::Color>& swaped_colors);
		void Apply();
		void Cancel();

	private:
		sf::Shader _shader;
		int old_shader;
	};
}

#endif // !PALLETE_H
