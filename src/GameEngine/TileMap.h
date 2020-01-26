#ifndef TILEMAP_H
#define TILEMAP_H

#include "Geometry.h"

#include <vector>
#include <functional>
#include <fstream>

namespace GameEngine
{
	using namespace Geometry;

	const static Vector Directions[] = {
		Vector::Zero,
		Vector::Left,
		Vector::Up,
		Vector::Down,
		Vector::Right
	};

	template<class T> class TileMap
	{
	public:
		TileMap(int width, int height)
		{
			_width = width;
			_height = height;

			_map = new T * [width];

			for (int x = 0; x < width; ++x)
			{
				_map[x] = new T[height];

				for (int i = 0; i < height; ++i)
					_map[x][i] = T();
			}
		}

		~TileMap()
		{
			for (int x = 0; x < _width; ++x)
				delete _map[x];
			delete _map;
		}

		inline void SetCell(int x, int y, T value)
		{
			assert(x < _width && y < _height && x >= 0 && y >= 0);
			_map[x][y] = value;
		}

		inline const T& GetCell(const Vector& point) const
		{
			return GetCell((int)point.X, (int)point.Y);
		}

		inline const T& GetCell(int x, int y) const
		{
			assert(x < _width && y < _height && x >= 0 && y >= 0);
			return _map[x][y];
		}

		void Clear(T value = T())
		{
			for (int x = 0; x < _width; ++x)
				for (int y = 0; y < _height; ++y)
					_map[x][y] = value;
		}

		inline int GetWidth() const
		{
			return _width;
		}

		inline int GetHeight() const
		{
			return _height;
		}

		void FillRect(int x1, int y1, int width, int height, T value)
		{
			for (int x = x1; x < x1 + width; ++x)
				for (int y = y1; y < y1 + height; ++y)
					SetCell(x, y, value);
		}

		void LoadFromString(std::map<char, T> dictionary, const std::string& str)
		{
			Clear();

			assert((_width * _height) == str.length());

			int i = 0;
			for (int y = 0; y < _height; ++y)
				for (int x = 0; x < _width; ++x)
					SetCell(x, y, dictionary[str[i++]]);
		}

		void LoadFromString(std::function<T(char)> fabric, const std::string& str)
		{
			Clear();

			assert((_width * _height) == str.length());

			int i = 0;
			for (int y = 0; y < _height; ++y)
				for (int x = 0; x < _width; ++x)
					SetCell(x, y, fabric(str[i++]));
		}

		void LoadFromFile(std::map<char, T> dictionary, const std::string& filePath)
		{
			std::ifstream file;

			file.open(filePath);

			if (!file.is_open())
				throw std::rutime_error("Can't load file: " + filePath);

			sdt::string str;
			for (int y = 0; y < _height; ++y)
			{
				std::getline(file, str);
				assert(str.length() == _width);

				for (int x = 0; x < _width; ++x)
				{
					assert(dictionary.find(str[x]) != dictionary.end());

					_map[x][y] = dictionary[str[x]];
				}
			}
		}

		bool InBounds(const Vector& cell) const
		{
			return cell.X >= 0 && cell.Y >= 0 && cell.X < _width
				&& cell.Y < _height;
		}

		std::vector<Vector> GetCells(T cellType)
		{
			std::vector<Vector> cells;

			for (int x = 0; x < _width; ++x)
				for (int y = 0; y < _height; ++y)
					if (_map[x][y] == cellType)
						cells.emplace_back(x, y);
			return cells;
		}

		std::vector<std::pair<Vector, T>> GetCells(const Rect& rect)
		{
			std::vector<std::pair<Vector, T>> cells;

			for (int x = rect.Left; x < rect.Right; ++x)
				for (int y = rect.Top; y < rect.Buttom; ++y)
					cells.push_back({ Vector(x, y), GetCell(x, y) });
			return cells;
		}

		Vector TraceLine(const Vector& start_cell, const Vector& direction, const std::function<bool(T)>& allowed_cell)
		{
			Vector curr_cell = Floor(start_cell);

			if (!allowed_cell(GetCell(curr_cell)))
				return curr_cell;

			//assert(allowed_cell(getCell(curr_cell)));

			if (direction == Vector::Zero)
				return curr_cell;

			while (InBounds(curr_cell) && allowed_cell(GetCell(curr_cell)))
				curr_cell += direction;

			curr_cell += -direction;

			assert(allowed_cell(GetCell(curr_cell)));
			return curr_cell;
		}

		Vector GetCell(const Vector& start_cell, const Vector& direction, int length)
		{
			Vector cur_cell = start_cell;
			for (int i = 0; i < length; ++i)
				cur_cell += direction;
			return cur_cell;
		}

		std::vector<Vector> GetNeighborNodes(const Vector& start_cell, const T& allowedCellType)
		{
			Vector curr_cell;
			std::vector<Vector> nodes;

			for (int i = 1; i < 5; ++i)
			{
				curr_cell = start_cell;
				while ((curr_cell == start_cell || getCellDegree(curr_cell, allowedCellType) < 3) && InBounds(curr_cell + directions[i]) && GetCell(curr_cell + directions[i]) == allowedCellType)
					curr_cell += directions[i];
				if (curr_cell != start_cell && curr_cell.x != 0 && curr_cell.x != m_width - 1)
					nodes.push_back(curr_cell);
			}

			return nodes;
		}

		TileMap& operator=(const TileMap& otherMap)
		{
			for (int x = 0; x < _width; ++x)
				for (int y = 0; y < _height; ++y)
					SetCell(x, y, otherMap.GetCell(x, y));
			_width = otherMap._width;
			_height = otherMap._heigth;

			return *this;
		}

	private:
		T** _map;
		int _height, _width;
	};

}
#endif // !TILEMAP_H
