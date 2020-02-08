#include "Geometry.h"

#include <cmath>

namespace GameEngine
{
	namespace Geometry
	{
		Vector RotateClockwise(const Vector& direction)
		{
			return Vector(-direction.Y, direction.X);
		}

		Vector RotateAntiClockwise(const Vector& direction)
		{
			return Vector(direction.Y, -direction.X);
		}

		int GetLength(const std::vector<Vector>& path)
		{
			int length = 0;
			
			for (int i = 1; i < path.size(); ++i)
				length += (path[i] - path[i - 1]).Lenght();
			return length;
		}

		Vector ToVector(const std::string& str)
		{
			if (str == "Left" || str == "left") return Vector::Left;
			if (str == "Right" || str == "right") return Vector::Right;
			if (str == "Down" || str == "down") return Vector::Down;
			if (str == "Up" || str == "up") return Vector::Up;
			return Vector::Zero;
		}

		Vector::Vector(float x, float y) : X(x), Y(y)
		{
			
		}

		Vector::Vector(int x, int y) : X((float)x), Y((float)y)
		{
			
		}

		Vector::Vector(const sf::Vector2f& vector)
		{
			X = vector.x;
			Y = vector.y;
		}

		Vector Vector::operator+(const Vector& right) const
		{
			return Vector(X + right.X, Y + right.Y);
		}

		Vector Vector::operator-(const Vector& right) const
		{
			return Vector(X - right.X, Y - right.Y);
		}

		Vector Vector::operator*(const float& k) const
		{
			return Vector(X * k, Y * k);
		}

		Vector Vector::operator/(const float& k) const
		{
			return Vector(X / k, Y / k);
		}

		void Vector::operator*=(const float& k)
		{
			X *= k;
			Y *= k;
		}

		void Vector::operator+=(const Vector& right)
		{
			X += right.X;
			Y += right.Y;
		}

		void Vector::operator-=(const Vector& right)
		{
			X -= right.X;
			Y -= right.Y;
		}

		Vector Vector::operator-() const
		{
			return Vector(-X, -Y);
		}

		bool Vector::operator==(const Vector& right) const
		{
			return (X == right.X && Y == right.Y);
		}

		bool Vector::operator!=(const Vector& right) const
		{
			return !(*this == right);
		}

		bool Vector::operator<(const Vector& right) const
		{
			return Y * 10000 + X < right.Y * 10000 + right.X;
		}

		float Vector::Lenght() const
		{
			return std::sqrt(X * X + Y * Y);
		}

		float Vector::Angle() const
		{
			float angle = std::atan2(X, Y) / PI * 180;
			if (angle < 0)
				angle += 360;
			return angle;
		}

		Vector::operator sf::Vector2f() const
		{
			return sf::Vector2f(X, Y);
		}

		Vector Vector::Normalized() const
		{
			return Vector(X / Lenght(), Y / Lenght());
		}

		void Vector::Normalize()
		{
			X /= Lenght();
			Y /= Lenght();
		}

		Vector Round(const Vector& vector)
		{
			return Vector(std::roundf(vector.X), std::roundf(vector.Y));
		}

		Vector Floor(const Vector& vector)
		{
			return Vector(std::floor(vector.X), std::floor(vector.Y));
		}

		Vector Vector::MoveToWards(const Vector& current, const Vector& target, float distance)
		{
			return (target - current).Normalized() * distance + current;
		}

		Vector operator*(const float& k, const Vector& vector)
		{
			return Vector(vector.X * k, vector.Y * k);
		}

		const Vector Vector::Right = { 1,0 };
		const Vector Vector::Left = { -1,0 };
		const Vector Vector::Up = { 0,-1 };
		const Vector Vector::Down = { 0,1 };
		const Vector Vector::Zero = { 0,0 };

		std::ostream& operator<<(std::ostream& os, const Vector& vector)
		{
			os << "[" << vector.X << "," << vector.Y << "]";
			return os;
		}

		Rect::Rect()
		{
			Left = Top = Width = Height = 0;
		}

		Rect::Rect(float left, float top, float width, float height) : Left(left), Top(top), Width(width), Height(height) 
		{

		}

		Rect::Rect(const Vector& top_left, const Vector& size) : Left(top_left.X), Top(top_left.Y), Width(size.X), Height(size.Y)
		{

		}

		bool Rect::IsContain(const Vector& point) const
		{
			return (point.X >= Left) && (point.Y >= Top) &&
				(point.X < Right()) && (point.Y < Bottom());
		}

		bool Rect::IsContain(const Rect& rect) const
		{
			return (rect.Left >= Left) && (rect.Right() <= Right()) &&
				(rect.Top >= Top) && (rect.Bottom() <= Bottom());
		}

		bool Rect::IsContainByX(const Vector& point) const
		{
			return (point.X >= Left) && (point.X < Right());
		}

		bool Rect::IsContainByY(const Vector& point) const
		{
			return (point.Y >= Top) && (point.Y < Bottom());
		}

		bool Rect::IsIntersect(const Rect& rect) const
		{
			return abs(2 * (Left - rect.Left) + Width - rect.Width) < abs(Width + rect.Width)
				&& abs(2 * (Top - rect.Top) + Height - rect.Height) < abs(Height + rect.Height);
		}

		Rect Rect::GetIntersection(const Rect& rect) const
		{
			float left, top, height, width;

			left = std::max(Left, rect.Left);
			top = std::max(Top, rect.Top);
			height = std::min(Bottom(), rect.Bottom()) - top;
			width = std::min(Right(), rect.Right()) - left;

			return Rect(left, top, width, height);
		}

		Rect Rect::Bordered(float k) const
		{
			return Rect(Left - k, Top - k, Width + 2 * k, Height + 2 * k);
		}

		Rect Rect::operator*(float k) const
		{
			return Rect(Left * k, Top * k, Width * k, Height * k);
		}

		bool Rect::operator==(const Rect& right) const
		{
			return (Left == right.Left) && (Top == right.Top) &&
				(Width == right.Width) && (Height == right.Height);
		}

		void Rect::SetLeft(int left)
		{
			Width += Left - left;
			Left = left;
		}

		void Rect::SetRight(int right)
		{
			Width = right - Left;
		}

		void Rect::SetTop(int top)
		{
			Height += Top - top;
			Top = top;
		}

		void Rect::SetBottom(int buttom)
		{
			Height = buttom - Top;
		}

		void Rect::Normalize()
		{
			if (Width < 0)
			{
				Left += Width;
				Width = -Width;
			}

			if (Height < 0)
			{
				Top += Height;
				Height = -Height;
			}
		}

		Rect Rect::Moved(const Vector& diff) const
		{
			return Rect(Left + diff.X, Top + diff.X, Width, Height);
		}

		float Rect::Right() const
		{
			return Left + Width;
		}

		float Rect::Bottom() const
		{
			return Top + Height;
		}

		Vector Rect::LeftTop() const
		{
			return Vector(Left, Top);
		}

		Vector Rect::RightTop() const
		{
			return Vector(Right(), Top);
		}

		Vector Rect::LeftButton() const
		{
			return Vector(Left, Bottom());
		}

		Vector Rect::RightBottom() const
		{
			return Vector(Right(), Bottom());
		}

		Vector Rect::Center() const
		{
			return Vector(Left + Width / 2, Top + Height / 2);
		}

		Vector Rect::Size() const
		{
			return Vector(Width, Height);
		}
	}
}