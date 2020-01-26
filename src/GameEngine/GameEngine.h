#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Geometry.h"
#include "TileMap.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cassert>
#include <sstream>
#include <functional>
#include <list>
#include <unordered_map>
#include <fstream>
#include <iostream>

namespace GameEngine
{
	template <typename T>
	std::string ToString(const T& param)
	{
		std::stringstream sstream;
		sstream << param;
		return sstream.str();
	}

	std::vector<std::string> Split(const std::string& str, char delim);

	float ToFloat(const std::string& str);
	int ToInt(const std::string& str);
	bool ToBool(const std::string& str);

	namespace math
	{
		int sign(float value);

		template <typename T>
		T clamp(const T& value, const T& min, const T& max)
		{
			if (value < min)
				return min;
			if (value > max)
				return max;
			return value;
		}

		template <typename T>
		T sens(const T& value, const T& lim)
		{
			if (value < lim && value > -lim)
				return 0;
			return value;
		}
	}

	enum class CollisionTag : int
	{
		None = 0x000000,
		Cell = 0x000001,
		Floor = 0x00002,
		Left = 0x00004,
		Right = 0x00008,
		Up = Cell,
		Down = Floor
	};

	inline CollisionTag operator|=(CollisionTag& one, const CollisionTag& two)
	{
		one = CollisionTag(static_cast<int>(one) | static_cast<int>(two));
		return one;
	}

	inline bool operator&(CollisionTag one, CollisionTag two)
	{
		return static_cast<int>(one) & static_cast<int>(two);
	}

	void DrawLinearSprite_v(sf::Sprite sprite, const sf::Rect<int>& draw_area, sf::RenderWindow* render_window);
	void DrawLinearSprite_h(sf::Sprite sprite, const sf::Rect<int>& draw_area, sf::RenderWindow* render_window);
	Vector CollsionResponse(const Rect& own_rect, const Vector& own_speed, const Rect& other_rect, const Vector& other_speed, const float delta_time, CollisionTag& collision_tag);
}
#endif // !GAME_ENGINE_H
