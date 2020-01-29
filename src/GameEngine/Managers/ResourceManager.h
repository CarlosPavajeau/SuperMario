#ifndef RESORUCE_MANAGER_H
#define RESORUCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"

#include <cassert>

namespace GameEngine
{
	namespace Managers
	{

		template<typename T>
		class ResourceManager
		{
		public:
			ResourceManager() = default;
			ResourceManager(const ResourceManager&) = delete;
			ResourceManager operator=(const ResourceManager&) = delete;
			~ResourceManager()
			{
				return;
				for (auto r : resources)
					if (r.second != nullptr)
					{
						delete r.second;
						r.second = nullptr;
					}
			}

			virtual void LoadFromFile(const std::string& name, const std::string& file_path)
			{
				assert(resources[name] == nullptr);

				resources[name] = new T();

				if (!resources[name]->loadFromFile(file_path))
					throw std::runtime_error(("Can't load resource: " + file_path).c_str());
			}
			T* Get(const std::string& name)
			{
				if (resources.find(name) == resources.end())
					std::cout << name.c_str() << std::endl;

				assert(resources.find(name) != resources.end()); //no such resource
				return resources[name];
			}
			T* operator[](const std::string& name)
			{
				return Get(name);
			}
			const T* Get(const std::string& name) const
			{
				assert(resources[name] != nullptr); //no such resource
				return resources[name];
			}
		protected:
			std::map<std::string, T*> resources;
		};

		template<>
		inline void ResourceManager<sf::Music>::LoadFromFile(const std::string& name, const std::string& file_path)
		{
			assert(resources[name] == nullptr); // allready exist
			resources[name] = new sf::Music();
			if (!resources[name]->openFromFile(file_path))
				throw std::runtime_error(("runtime error can't load resource: " + file_path).c_str());
			resources[name]->setLoop(true);
		}

		using TextureManager = ResourceManager<sf::Texture>;
		using FontManager = ResourceManager<sf::Font>;
		using SoundManager = ResourceManager<sf::SoundBuffer>;

	}
}
#endif // !RESORUCE_MANAGER_H
