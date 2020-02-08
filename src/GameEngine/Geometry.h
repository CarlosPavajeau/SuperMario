#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "SFML/Graphics.hpp"

#include <unordered_map>
#include <queue>
#include <map>
#include <iostream>
#include <cassert>

namespace GameEngine
{
	namespace Geometry
	{
		#define PI 3.14159265

		struct Vector
		{
			Vector()
			{
				X = Y = 0;
			}

			Vector(float x, float y);
			Vector(int x, int y);

			Vector(const sf::Vector2f& vector);
			operator sf::Vector2f() const;

			Vector operator-() const;
			Vector operator+(const Vector& right) const;
			Vector operator-(const Vector& right) const;
			Vector operator*(const float& k) const;
			Vector operator/(const float& k) const;

			void operator+=(const Vector& right);
			void operator-=(const Vector& right);
			void operator*=(const float& k);

			bool operator==(const Vector& right) const;
			bool operator!=(const Vector& right) const;
			bool operator<(const Vector& right) const;

			float Lenght() const;
			float Angle() const;

			Vector Normalized() const;
			void Normalize();

			static Vector MoveToWards(const Vector& current, const Vector& target, float distance);

			const static Vector Right;
			const static Vector Left;
			const static Vector Up;
			const static Vector Down;
			const static Vector Zero;

			float X, Y;
		};

		std::ostream& operator<<(std::ostream& os, const Vector& vector);

		Vector ToVector(const std::string& vectordirection);
		Vector RotateClockwise(const Vector& direction);
		Vector RotateAntiClockwise(const Vector& direction);

		Vector operator*(const float& k, const Vector& vector);
		Vector Round(const Vector& vector);
		Vector Floor(const Vector& vector);

		struct Rect
		{
			Rect();
			Rect(float left, float top, float width, float height);
			Rect(const Vector& top_left, const Vector& size);

			bool IsContain(const Vector& point) const;
			bool IsContain(const Rect& rect) const;
			bool IsContainByX(const Vector& point) const;
			bool IsContainByY(const Vector& point) const;
			bool IsIntersect(const Rect& rect) const;

			Rect GetIntersection(const Rect& rect) const;
			Rect Bordered(float k) const;

			Rect operator*(float k) const;
			bool operator==(const Rect& rect) const;

			void SetLeft(int left);
			void SetRight(int right);
			void SetBottom(int buttom);
			void SetTop(int top);

			void Normalize();
			Rect Moved(const Vector& diff) const;

			float Right() const, Bottom() const;
			Vector LeftTop() const, RightTop() const, LeftButton() const, RightBottom() const,
				Center() const, Size() const;

			float Left, Top, Width, Height;
		};
	}
}

namespace std
{
	template<>
	struct hash<GameEngine::Geometry::Vector>
	{
		size_t operator()(const GameEngine::Geometry::Vector& vector) const
		{
			return size_t(vector.Y) * 10000 + size_t(vector.X);
		}
	};
}

#endif // !GEOMETRY_H
