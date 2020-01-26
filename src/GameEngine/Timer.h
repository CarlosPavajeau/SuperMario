#include <list>
#include <functional>

#ifndef TIMER_H
#define TIMER_H

namespace GameEngine
{
	class Timer
	{
	public:
		void Invoke(const std::function<void()>& func, int delay);
		void Update(int deltaTime);
		void Pause();
		void Play();
	private:
		int _time;
		bool _paused = false;
		std::list<std::pair<int, std::function<void()>>> _invoke_list;
	};

}
#endif // !TIMER_H

