#ifndef GAME_H
#define GAME_H

#include "Geometry.h"
#include "ResourceManager.h"
#include "MusicManager.h"
#include "InputManager.h"
#include "EventManager.h"

#include <string>

namespace GameObjects
{
	class GameObject;
}

namespace GameEngine
{
	using namespace Geometry;
	using namespace Managers;
	using namespace Events;

	class Game
	{
	public:
		Game(const std::string& name, const Vector& screen_size);
		~Game() { }

		void Run();

		GameObjects::GameObject* GetRootObject() { return _root_object; }

		TextureManager& TextureManager() { return textureManager; }
		FontManager& FontManager() { return fontManager; }
		SoundManager& SoundManager() { return soundManager; }
		EventManager& EventManager() { return eventManager; }
		InputManager& InputManager() { return inputManager; }
		MusicManager& MusicManager() { return musicManager; }

		void PlaySound(const std::string& name);
		void PlayMusic(const std::string& name);
		void StopMusic();
		Vector ScreenSize() const { return screen_size; }

	protected:
		void virtual Init() { }
		void virtual Update(int delta_time);
		void SetClearColor(const sf::Color& color);

	private:
		GameObjects::GameObject* _root_object = nullptr;
		Managers::TextureManager textureManager;
		Managers::FontManager fontManager;
		Managers::SoundManager soundManager;
		Managers::MusicManager musicManager;
		Managers::InputManager inputManager;
		Events::EventManager eventManager;

		sf::Sound sounds_buff[40];
		sf::RenderWindow* window = nullptr;

		Vector screen_size;
		sf::Color clearColor = sf::Color::Black;

		sf::Time minTime = sf::seconds(3600);
		sf::Time maxTime = sf::Time::Zero;

		void Draw(sf::RenderWindow* renderWindow);
		void UpdateStats(const sf::Time time);
	};
}


#endif // !GAME_H
