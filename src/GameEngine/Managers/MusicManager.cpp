#include "MusicManager.h"

namespace GameEngine
{
	namespace Managers
	{
		void MusicManager::Play(const std::string& name)
		{
			if (name != "")
				_currentMusic = name;

			assert(resources[_currentMusic]);

			if (!_currentMusic.empty())
				resources[_currentMusic]->play();
		}

		void MusicManager::Stop()
		{
			for (auto music : resources)
				music.second->stop();
			_currentMusic.clear();
		}

		void MusicManager::Pause()
		{
			for (auto music : resources)
				music.second->pause();
		}

		void MusicManager::SetPitch(float value)
		{
			for (auto music : resources)
				music.second->setPitch(value);
		}
	}
}