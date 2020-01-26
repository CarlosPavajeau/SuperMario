#include "Timer.h"

namespace GameEngine
{
	void Timer::Invoke(const std::function<void()>& func, int delay)
	{
		_invoke_list.push_back({ _time + delay, func });
	}

	void Timer::Update(int deltaTime)
	{
		if (!_paused)
		{
			_time += deltaTime;

			for (auto itr = _invoke_list.begin(); itr != _invoke_list.end();)
			{
				if (_time > itr->first)
				{
					itr->second();
					itr = _invoke_list.erase(itr);
				}
				else
					++itr;
			}
		}
	}

	void Timer::Pause()
	{
		_paused = true;
	}

	void Timer::Play()
	{
		_paused = false;
	}
}