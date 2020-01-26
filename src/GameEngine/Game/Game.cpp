#include "Game.h"

namespace GameEngine
{
	Game::Game(const std::string& name, const Vector& screen_size) : screen_size(screen_size)
	{
		_root_object = new GameObjects::GameObject();
		_root_object->SetName(name);
	}

	void Game::Run()
	{
		window = new sf::RenderWindow(sf::VideoMode(screen_size.X, screen_size.Y), _root_object->GetName());
		Init();

		sf::Event event;
		sf::Clock clock;
		sf::Time acumulator = sf::Time::Zero;
		const sf::Time ups = sf::seconds(1.f / 60.f);

		while (true)
		{
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::EventType::Closed)
				{
					window->close();
					exit(0);
				}

				EventManager().PushEvent(event);
			}

			sf::Time elapsedTime = clock.restart();
			acumulator += elapsedTime;

			while (acumulator > ups)
			{
				acumulator -= ups;
				InputManager().Update(ups.asMilliseconds());
				Update(ups.asMilliseconds());
				sf::sleep(sf::milliseconds(10));
			}

			window->clear(clearColor);
			Draw(window);
			window->display();
		}
	}

	void Game::PlaySound(const std::string& name)
	{
		const int SOUND_BUFFER_SIZE = 40;
		int i = 0;

		while (sounds_buff[i].getStatus() == sf::Sound::Playing)
		{
			if (i >= SOUND_BUFFER_SIZE)
				break;
			++i;
		}

		if (i >= SOUND_BUFFER_SIZE)
			throw std::runtime_error("Sound buffer overflow");

		sounds_buff[i].setBuffer(*SoundManager().Get(name));
		sounds_buff[i].play();
	}

	void Game::PlayMusic(const std::string& name)
	{
		MusicManager().Play(name);
	}

	void Game::StopMusic()
	{
		MusicManager().Stop();
	}

	void Game::Update(int delta_time)
	{
		GameObjects::GameObject::InvokePreupdateActions();
		_root_object->Update(delta_time);
	}

	void Game::SetClearColor(const sf::Color& color)
	{
		clearColor = color;
	}

	void Game::Draw(sf::RenderWindow* renderWindow)
	{
		_root_object->Draw(renderWindow);
		_root_object->PostDraw(renderWindow);
	}

	void Game::UpdateStats(const sf::Time time)
	{
		if (time > maxTime)
			maxTime = time;
		if (time < minTime)
			minTime = time;

		static int k = 0;

		if (++k % 60 == 0)
		{
			std::cout << "min: " << minTime.asMilliseconds() << " max: " << maxTime.asMilliseconds() << std::endl;
			minTime = sf::seconds(3600);
			maxTime = sf::Time::Zero;
		}
	}
}