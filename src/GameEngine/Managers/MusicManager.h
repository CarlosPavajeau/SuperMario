#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "ResourceManager.h"

#include <SFML/Audio.hpp>

namespace GameEngine
{
	namespace Managers
	{

		class MusicManager : public ResourceManager<sf::Music>
		{
		public:
			void Play(const std::string& name = "");
			void Stop();
			void Pause();
			void SetPitch(float value);

		private:
			std::string _currentMusic;
		};
	}
}

#endif // !MUSIC_MANAGER_H
