#include "EventManager.h"

namespace GameEngine
{
	namespace Events
	{
		void EventManager::PushEvent(const sf::Event& event)
		{
			for (auto& sub : _subcribes)
				sub->Events(event);
		}

		void EventManager::Subscribe(GameObjects::GameObject* object)
		{
			_subcribes.push_back(object);
		}

		void EventManager::UnSubcribe(GameObjects::GameObject* object)
		{
			auto it = std::find(_subcribes.begin(), _subcribes.end(), object);
			if (it != _subcribes.end())
				_subcribes.erase(it);
		}
	}
}