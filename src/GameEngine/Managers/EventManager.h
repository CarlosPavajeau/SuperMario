#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace GameEngine
{
	namespace Events
	{
		class EventManager
		{
		public:
			void PushEvent(const sf::Event& event);
			void Subscribe(GameObjects::GameObject* object);
			void UnSubcribe(GameObjects::GameObject* object);

		private:
			std::list<GameObjects::GameObject*> _subcribes;
		};
	}
}

#endif // !EVENT_MANAGER_H
